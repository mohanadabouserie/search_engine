# search_engine
This repository contains the source code for a simple web search engine. The search engine takes user queries as input and returns a list of web pages ranked based on relevance.

## Sample Search Queries
The following are five sample search query inputs and their corresponding outputs:

1. Input: wild
   - Output:
    1. www.test24.com
    2. www.test4.com

Input: "sports complex"
Output:
www.test11.com
www.test2.com
www.test1.com

Input: browny OR bumpy
Output:
www.test5.com
www.test8.com
www.test23.com
www.test15.com
www.test20.com

Input: argument AND bumpy
Output:
www.test5.com
www.test23.com

Input: bumpy browny
Output:
www.test5.com
www.test8.com
www.test23.com
www.test15.com
www.test20.com

## Implementation Details
The search engine is implemented using C++. It consists of a Graph class that represents the web graph and contains methods for building the graph, calculating page ranks, CTR, and scores, and performing searches based on user queries.


## Usage
To use the search engine, follow these steps:

- Build the graph by reading the web graph data from the web_graph.csv file.
- Calculate the click-through rates (CTR) of each web page based on the data in the number_of_impressions.csv file.
- Calculate the relevance scores of each web page based on a combination of page rank and CTR.
- Read the keywords and create a map of keywords to web pages.
- Perform a search by providing a query as input to the search method.
- View the search results and choose a webpage to open or perform a new search.

## File Structure

The repository contains the following files:
1. web_graph.csv: Contains the web graph data in the format u,v representing an edge from web page u to web page v.
2. number_of_impressions.csv: Contains the number of impressions and clicks for each web page.
3. keywords.csv: Contains the keywords associated with each web page.
4. main.cpp: Contains the main code for the search engine.
