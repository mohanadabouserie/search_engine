/*
Those are five sample search query inputs and their corresponding outputs:

1)
input:
wild
output:
1. www.test24.com
2. www.test4.com

2)
input:
"sports complex"
ouput:
1. www.test11.com
2. www.test2.com
3. www.test1.com

3)
input:
browny OR bumpy
output:
1. www.test5.com
2. www.test8.com
3. www.test23.com
4. www.test15.com
5. www.test20.com

4)
input:
argument AND bumpy
output:
1. www.test5.com
2. www.test23.com

5)
input:
bumpy browny
output:
1. www.test5.com
2. www.test8.com
3. www.test23.com
4. www.test15.com
5. www.test20.com
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Graph
{
private:
    vector<vector<int>> adj;
    vector<double> pageRank;
    vector<double> impressions;
    vector<double> clicks;
    vector<double> CTR;
    vector<double> score;
    map<string, set<int>> keywordMap;

public:
    Graph(int V)
    {
        adj.resize(V+1);
        pageRank.resize(V+1);
        impressions.resize(V+1);
        clicks.resize(V+1);
        CTR.resize(V+1);
        score.resize(V+1);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
    }

    void printGraph()
    {
        for(int i = 1; i < adj.size(); i++)
        {
            cout << i << " -> ";
            for(int j = 0; j < adj[i].size(); j++)
            {
                cout << adj[i][j] << " ";
            }
            cout << endl;
        }
    }

    void buildGraphFromFile()
    {
        ifstream infile("web_graph.csv");
        string line;
        while(getline(infile, line))
        {
            stringstream ss(line);
            string urls;
            getline(ss, urls);
            int u,v;
            extractNumbers(urls, u, v);
            addEdge(u, v);
        }
        infile.close();
    }

    void extractNumbers(string line, int& u, int& v)
    {
        stringstream ss(line);
        string str1, str2;
        char ch;

        getline(ss, str1, ',');
        stringstream ss1(str1);
        while (ss1 >> ch)
        {
            if (isdigit(ch))
            {
                ss1.putback(ch);
                ss1 >> u;
            }
        }

        getline(ss, str2, ',');
        std::stringstream ss2(str2);
        while (ss2 >> ch)
        {
            if (isdigit(ch))
            {
                ss2.putback(ch);
                ss2 >> v;
            }
        }
    }

    void CalculateCTR()
    {
        ifstream infile("number_of_impressions.csv");
        string line;
        int page = 1;
        while (getline(infile, line))
        {
            stringstream ss(line);
            string url, impressionStr, clickStr;
            getline(ss, url, ',');
            getline(ss, impressionStr, ',');
            getline(ss, clickStr, ',');
            int impression = stoi(impressionStr);
            int click = stoi(clickStr);
            impressions[page] = impression;
            clicks[page] = click;
            CTR[page] = (click * 1.0) / impression;
            page++;
        }
        infile.close();
    }

    void update_score()
    {
        for (int page = 1; page < impressions.size(); page++)
        {
            CTR[page] = (clicks[page] * 1.0) / impressions[page];
        }
        for(int i = 1; i < score.size(); i++)
        {
            score[i] = 0.4 * pageRank[i] + ((1.0-((0.1*impressions[i])/(1+0.1*impressions[i])))*pageRank[i]+((0.1*impressions[i])/(1+0.1*impressions[i]))*CTR[i])*0.4;
        }
    }

    void print_CTR()
    {
        for(int i = 1; i < impressions.size(); i++)
        {
            cout << "Page " << i << ": " << impressions[i] << "    " << clicks[i] << "    " << CTR[i] << endl;
        }
    }

    void calculate_score()
    {
        this->calculatePageRank();
        this->CalculateCTR();
        for(int i = 1; i < score.size(); i++)
        {
            score[i] = 0.4 * pageRank[i] + ((1.0-((0.1*impressions[i])/(1+0.1*impressions[i])))*pageRank[i]+((0.1*impressions[i])/(1+0.1*impressions[i]))*CTR[i])*0.4;
        }
    }

    void print_score()
    {
        for(int i = 1; i < score.size(); i++)
        {
            cout << "Page " << i << ": " << score[i] << endl;
        }
    }

    void calculatePageRank()
    {
        double d = 0.85;
        int maxIter = 100;
        double eps = 1e-8;
        int V = adj.size();
        vector<double> prevPageRank(V, 1.0 / V);

        for (int iter = 0; iter < maxIter; iter++)
        {
            vector<double> currPageRank(V, 0.0);

            for (int i = 1; i < V; i++)
            {
                for (int j = 0; j < adj[i].size(); j++)
                {
                    int v = adj[i][j];
                    currPageRank[v] += prevPageRank[i] / adj[i].size();
                }
            }

            double sum = 0.0;
            for (int i = 0; i < V; i++)
            {
                currPageRank[i] = d * currPageRank[i] + (1 - d) / V;
                sum += abs(currPageRank[i] - prevPageRank[i]);
            }

            if (sum < eps)
            {
                break;
            }

            prevPageRank = currPageRank;
        }

        pageRank = prevPageRank;
    }

    void printPageRank()
    {
        for(int i = 1; i < pageRank.size(); i++)
        {
            cout << "Page " << i << ": " << pageRank[i] << endl;
        }
    }

    void readKeywords()
    {
        ifstream infile("keywords.csv");
        string line;
        int page = 1;
        while (getline(infile, line))
        {
            stringstream ss(line);
            string url, keyword;
            getline(ss, url, ',');
            while (getline(ss, keyword, ','))
            {
                keywordMap[keyword].insert(page);
            }
            page++;
        }
        infile.close();
    }

    void printKeywordsMap()
    {
        for (const auto& entry : keywordMap)
        {
            cout << "Keyword: " << entry.first << endl;
            cout << "Websites: ";
            for (int website : entry.second)
            {
                cout << website << " ";
            }
            cout << endl << endl;
        }
    }


    void search(string query)
    {
        try
        {
            set<int> pages;
            if(query[0] == '"' && query[query.size()-1] == '"')
            {
                query = query.substr(1, query.size() - 2);
                auto it = keywordMap.find(query);

                if (it != keywordMap.end())
                {
                    for (int number : it->second)
                    {
                        pages.insert(number);
                    }
                }
                else
                {
                    cout << endl << "No results found" << endl << endl;
                    return;
                }
            }
            else
            {
                stringstream ss(query);
                string keyword;
                vector<string> keywords;
                while (ss >> keyword)
                {
                    keywords.push_back(keyword);
                }
                if (keywords.size() == 1)
                {
                    auto it = keywordMap.find(keywords[0]);
                    if (it != keywordMap.end())
                    {
                        for (int number : it->second)
                        {
                            pages.insert(number);
                        }
                    }
                    else
                    {
                        cout << endl << "No results found" << endl << endl;
                        return;
                    }
                }
                else if(keywords.size() == 2)
                {
                    auto it = keywordMap.find(keywords[0]);
                    if (it != keywordMap.end())
                    {
                        for (int number : it->second)
                        {
                            pages.insert(number);
                        }
                    }
                    it = keywordMap.find(keywords[1]);
                    if (it != keywordMap.end())
                    {
                        for (int number : it->second)
                        {
                            pages.insert(number);
                        }
                    }
                    if(keywordMap.find(keywords[0]) == keywordMap.end() && keywordMap.find(keywords[1]) == keywordMap.end())
                    {
                        cout << endl << "No results found" << endl << endl;
                        return;
                    }
                }
                else if(keywords.size() == 3 && (keywords[1] == "AND" || keywords[1] == "OR"))
                {
                    if(keywords[1] == "OR")
                    {
                        auto it = keywordMap.find(keywords[0]);
                        if (it != keywordMap.end())
                        {
                            for (int number : it->second)
                            {
                                pages.insert(number);
                            }
                        }
                        it = keywordMap.find(keywords[2]);
                        if (it != keywordMap.end())
                        {
                            for (int number : it->second)
                            {
                                pages.insert(number);
                            }
                        }
                        if(keywordMap.find(keywords[0]) == keywordMap.end() && keywordMap.find(keywords[2]) == keywordMap.end())
                        {
                            cout << endl << "No results found" << endl << endl;
                            return;
                        }
                    }
                    else if(keywords[1] == "AND")
                    {
                        set<int> set1, set2;
                        auto it = keywordMap.find(keywords[0]);
                        if (it != keywordMap.end())
                        {
                            for (int number : it->second)
                            {
                                set1.insert(number);
                            }
                        }
                        it = keywordMap.find(keywords[2]);
                        if (it != keywordMap.end())
                        {
                            for (int number : it->second)
                            {
                                set2.insert(number);
                            }
                        }

                        if(keywordMap.find(keywords[0]) == keywordMap.end() || keywordMap.find(keywords[2]) == keywordMap.end())
                        {
                            cout << endl << "No results found" << endl << endl;
                            return;
                        }
                        else
                        {
                            set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(pages, pages.begin()));
                        }
                    }
                }
                else
                {
                    throw "Error";
                }
            }
            vector<pair<int, double>> pageScores;
            for (auto element : pages)
            {
                pageScores.push_back(make_pair(element, score[element]));
            }
            sort(pageScores.begin(), pageScores.end(), [](const auto& lhs, const auto& rhs) { return lhs.second > rhs.second; });
            for (auto element : pageScores)
            {
                impressions[element.first]++;
            }
            string respond = "1";
            while(respond == "1" || respond == "Back to search results")
            {
                pageScores.clear();
                for (auto element : pages)
                {
                    pageScores.push_back(make_pair(element, score[element]));
                }
                sort(pageScores.begin(), pageScores.end(), [](const auto& lhs, const auto& rhs) { return lhs.second > rhs.second; });
                string resp;
                cout << endl << "Search results: " << endl;
                int k = 1;
                for (auto element : pageScores)
                {
                    cout << k << ". www.test" << element.first << ".com" << endl;
                    k++;
                }
                cout << endl << "Would you like to " << endl;
                cout << "1. Choose a webpage to open" << endl << "2. New search" << endl << "3. Exit" << endl << "Type in your choice: ";
                getline(cin, resp);
                while(resp != "Exit" && resp != "New search" && resp != "Choose a webpage to open" && resp != "1" && resp != "2" && resp != "3")
                {
                    cout << endl << "Invalid choice!" << endl;
                    cout << "Would you like to " << endl;
                    cout << "1. Choose a webpage to open" << endl << "2. New search" << endl << "3. Exit" << endl << "Type in your choice: ";
                    getline(cin, resp);
                }

                if(resp == "3" || resp == "Exit")
                {
                    this->update_score();
                    this->update_file();
                    exit(1);
                }
                else if(resp == "2" || resp == "New search")
                {
                    this->update_score();
                    return;
                }
                else if(resp == "1" || resp == "Choose a webpage to open")
                {
                    string choice;
                    cout << endl << "Type in your webpage choice: ";
                    getline(cin, choice);
                    while(pages.find(stoi(choice)) == pages.end())
                    {
                        cout << endl << "Invalid choice!" << endl;
                        cout << "Type in your webpage choice: ";
                        getline(cin, choice);
                    }
                    cout << endl << "You're now viewing www.test" << choice << ".com" << endl;
                    clicks[stoi(choice)]++;
                    this->update_score();
                    cout << "Would you like to " << endl;
                    cout << "1. Back to search results" << endl << "2. New search" << endl << "3. Exit" << endl << "Type in your choice: ";
                    getline(cin, respond);
                    while(respond != "Exit" && respond != "New search" && respond != "Back to search results" && respond != "1" && respond != "2" && respond != "3")
                    {
                        cout << "Would you like to " << endl;
                        cout << "1. Back to search results" << endl << "2. New search" << endl << "3. Exit" << endl << "Type in your choice: ";
                        getline(cin, respond);
                    }

                    if(respond == "Exit" || respond == "3")
                    {
                        this->update_file();
                        exit(1);
                    }
                    else if(respond == "New search" || respond == "2")
                    {
                        return;
                    }
                }
            }

        }
        catch(...)
        {
            cout << "Invalid search query!" << endl;
        }
    }
    void update_file()
    {
        ofstream file("number_of_impressions.csv");
        if (file.is_open())
        {
            for (int i = 1; i < impressions.size(); ++i)
            {
                file << "www.test" << i << ".com," << impressions[i] << "," << clicks[i] << endl;
            }
            file.close();
        }
        else
        {
            cout << "Unable to save impressions and clicks to file." << endl;
        }
    }
};

void run_code()
{
    Graph g(30);
    g.buildGraphFromFile();
    g.calculate_score();
    g.readKeywords();
    string query;
    cout << "Welcome!" << endl;
    cout << "What would you like to do?" << endl << "1. New search" << endl << "2. Exit" << endl;
    cout << "Type in your choice: ";
    string response;
    getline(cin, response);
    while(response != "Exit" && response != "New search" && response != "1" && response != "2")
    {
        cout << endl << "Invalid input!" << endl;
        cout << "What would you like to do?" << endl << "1. New search" << endl << "2. Exit" << endl;
        cout << "Type in your choice: ";
        getline(cin, response);
    }
    while(response != "Exit" && response != "2")
    {
        cout << "Enter your search query: ";
        getline(cin, query);
        g.search(query);
    }
    g.update_file();
}

int main()
{
   run_code();
}