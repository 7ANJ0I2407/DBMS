#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

const int REGIONS = 4;
const int SALESMEN = 6;
const int PRODUCTS = 8;

struct SalesData
{
    int regionNo;
    int salesmanNo;
    int productCode;
    int unitsSold;
};

void generateSalesData(const string &input, int n)
{
    ofstream file(input);
    if (!file.is_open())
    {
        exit(1);
    }

    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < n; ++i)
    {
        int regionNo = rand() % REGIONS + 1;
        int salesmanNo = rand() % SALESMEN + 1;
        int productCode = rand() % PRODUCTS + 1;
        int unitsSold = rand() % 100 + 1;

        file << regionNo << " " << salesmanNo << " " << productCode << " " << unitsSold << endl;
    }
    file.close();
}

vector<SalesData> readSalesData(const string &file)
{
    ifstream inFile(file);
    if (!inFile.is_open())
    {
        exit(1);
    }

    vector<SalesData> sales;
    SalesData sale;
    while (inFile >> sale.regionNo >> sale.salesmanNo >> sale.productCode >> sale.unitsSold)
    {
        sales.push_back(sale);
    }

    inFile.close();
    return sales;
}

void generateSalesReport(const vector<SalesData> &sales, const string &output, const float unitPrice[])
{
    ofstream file(output);

    if (!file.is_open())
    {
        exit(1);
    }

    float totalRegion[REGIONS + 1] = {0}; // for taking the whole cost of the region

    for (int region = 1; region <= REGIONS; ++region)
    {
        file << "Region " << region << endl;
        file << "-----------------------" << endl;

        map<pair<int, int>, int> saleCount;

        for (auto sale : sales)
        {
            if (sale.regionNo == region)
            {
                int totalCost = unitPrice[sale.productCode - 1] * sale.unitsSold;
                saleCount[{sale.salesmanNo, sale.productCode}] += totalCost; // storing the total cost of the product sold by the salesman
            }
        }
        int totalRegionCost = 0;
        map<int, int> totalSalesmanCost;
        for (auto it : saleCount)
        {
            totalSalesmanCost[it.first.first] += it.second;
        }

        for (auto it : saleCount)
        {
            totalRegionCost += it.second;
        }
        for (auto it : totalSalesmanCost)
        {
            file << "Salesman " << it.first << " Rs. " << it.second << "/-" << endl;
        }
        file << "-----------------------" << endl;
        file << "Product wise sale" << endl;
        file << "-----------------------" << endl;

        for (int salesman = 1; salesman <= SALESMEN; ++salesman)
        {
            file << "Salesman " << salesman << endl;
            file << "-----------" << endl;
            for (int product = 1; product <= PRODUCTS; ++product)
            {
                if (saleCount.find({salesman, product}) != saleCount.end())
                {
                    file << "Product " << product << " : " << saleCount[{salesman, product}] / unitPrice[product - 1] << endl;
                }
            }
            file << "-----------------------" << endl;
        }
        file << "Total sale at Region " << region << " Rs. " << totalRegionCost << "/-" << endl;
        file << "--------------------------------------------------------------------------------------------------------" << endl;
    }
    file.close();
}

int main()
{
    string input = "sales.txt";
    int n;

    cout << "Enter the number of data entries : ";
    cin >> n;

    generateSalesData(input, n);

    vector<SalesData> sales = readSalesData(input);

    float productPrices[PRODUCTS] = {100, 200, 300, 400, 500, 600, 700, 800};

    generateSalesReport(sales, "sales_report_final.txt", productPrices);

    cout << "Report Generated." << endl;

    return 0;
}
