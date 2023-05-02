#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

struct Product {
    string name;
    int price;
    int quantity;
    int bought;
    int sold;
    vector<int> purchase_history;
    vector<int> order_history;
};

struct Order
{
    // roduct ID, product name, quantity, price, cost of goods sold (COGS) and selling price.  
    string id;
    string name;
    int quantity;
    int price;
    int cogs;
    int sellingPrice;

};


unordered_map<string, Product> catalog;
vector<Order> orders;

int get_average_price(string product_id) {
    if (catalog.count(product_id) == 0) {
        cout << "Error: Product " << product_id << " not found." << endl;
        return 0;
    }
    if (catalog[product_id].purchase_history.size() == 0) {
        cout << "Error: No purchase history for product " << product_id << "." << endl;
        return 0;
    }
    int total_price = 0;
    for (int price : catalog[product_id].purchase_history) {
        total_price += price;
    }
    int average_price = total_price / catalog[product_id].bought;

    return average_price;
}

void save_product(string product_id, string product_name, int price) {
    if (catalog.count(product_id) == 0) {
        catalog[product_id] = {product_name, price, 0, {}, {}};
    } else {
        catalog[product_id].name = product_name;
        catalog[product_id].price = price;
    }
    // cout << "Product " << product_id << " saved/updated." << endl;
}

void purchase_product(string product_id, int quantity, int price) {
    if (catalog.count(product_id) == 0) {
        cout << "Error: Product " << product_id << " not found." << endl;
        return;
    }
    catalog[product_id].quantity += quantity;
    catalog[product_id].bought += quantity;
    catalog[product_id].purchase_history.push_back(quantity*price);
    // cout << quantity << " units of product " << product_id << " purchased." << endl;
}

void order_product(string product_id, int quantity) {
    if (catalog.count(product_id) == 0) {
        cout << "Error: Product " << product_id << " not found." << endl;
        return;
    }
    if (catalog[product_id].quantity < quantity) {
        cout << "Error: Insufficient quantity of product " << product_id << "." << endl;
        return;
    }
    catalog[product_id].quantity -= quantity;
    catalog[product_id].sold += quantity;
    catalog[product_id].order_history.push_back(quantity*catalog[product_id].price);

    Order o;
    o.id = product_id;
    o.name = catalog[product_id].name;
    o.price = get_average_price(product_id);
    o.quantity = quantity;
    o.sellingPrice = catalog[product_id].price;
    int total_sell = 0;
    for (int price : catalog[product_id].order_history) {
        total_sell += price;
    }
    o.cogs = total_sell;
    orders.push_back(o);
    

    // cout << quantity << " units of product " << product_id << " ordered." << endl;
}

void get_quantity_of_product(string product_id) {
    if (catalog.count(product_id) == 0) {
        cout << "Error: Product " << product_id << " not found." << endl;
        return;
    }
    // cout << "Quantity of product " << product_id << ": " << catalog[product_id].quantity << endl;
    cout << catalog[product_id].quantity << endl;
}

void get_product_profit(string product_id) {
    if (catalog.count(product_id) == 0) {
        cout << "Error: Product " << product_id << " not found." << endl;
        return;
    }
    if (catalog[product_id].purchase_history.size() == 0) {
        cout << "Error: No purchase history for product " << product_id << "." << endl;
        return;
    }
    if (catalog[product_id].order_history.size() == 0) {
        cout << "Error: No order history for product " << product_id << "." << endl;
        return;
    }
    int total_price = 0;
    for (int price : catalog[product_id].purchase_history) {
        total_price += price;
    }
    double average_price = static_cast<double>(total_price) / catalog[product_id].bought;


    int total_sell = 0;
    for (int price : catalog[product_id].order_history) {
        total_sell += price;
    }
    double average_sell = static_cast<double>(total_sell) / catalog[product_id].sold;

    double profit = (average_sell-average_price)*catalog[product_id].sold;


    // cout << "Product profit: " << profit << endl;

    cout << profit << endl;

}

void get_fewest_product(){
    string res = "";
    int min = INT32_MAX;
    for(auto index : catalog){
        if(index.second.quantity<min){
            res = index.first;
            min = index.second.quantity;
        }
    }
    // cout<<"fewest product: "<<res<<endl;
    cout<<res<<endl;
}

void get_most_popular_product(){
    string res = "";
    int max = INT32_MIN;
    for(auto index : catalog){
        if(index.second.sold>max){
            res = index.first;
            max= index.second.sold;
        }
    }
    // cout<<"most popular product: "<<res<<endl;
    cout<<res<<endl;


}

void get_orders_report(){
    for(int i=0; i<orders.size(); i++){
        Order o = orders[i];
        cout<<"id: "<<o.id<<" name: "<<o.name<<" quantity: "<<o.quantity<<" price: "<<o.price<<
        " cost of goods sold: "<<o.cogs<<" selling price: "<<o.sellingPrice<<endl;
    }
}

void export_orders_report(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: could not open file " << filename << endl;
        return;
    }

    outFile << "Product ID,Product Name,Quantity,Price,COGS,Selling Price" << endl;

    for (const auto& order : orders) {
        stringstream ss;
        ss << order.id << "," << order.name << ","
           << order.quantity << "," << order.price << ","
           << order.cogs << "," << order.sellingPrice << endl;

        outFile << ss.str();
    }
    outFile.close();

    cout << "Orders exported to " << filename << endl;
}

int main() {

    while(1){
        string input;
        getline(cin, input);  
        istringstream iss(input);
        queue<std::string> q;

        string word;

        while (getline(iss, word, ' ')) {
            q.push(word);
        }   
        
        if(q.front()=="exit") break; //exit

        else if(q.front()=="save_product"){ //save prod
            q.pop();
            string id = q.front();
            q.pop();
            string name = q.front();
            q.pop();
            int price = stoi(q.front());
            save_product(id, name, price);
        }
    
        else if(q.front()=="purchase_product"){ //purchase_product
            q.pop();
            string id = q.front();
            q.pop();
            int quantity = stoi(q.front());
            q.pop();
            int price = stoi(q.front());
            purchase_product(id, quantity, price);
        }

        else if(q.front()=="order_product"){ //order_product
            q.pop();
            string id = q.front();
            q.pop();
            int quantity = stoi(q.front());
            order_product(id, quantity);
        }

        else if(q.front()=="get_quantity_of_product"){ //get_quantity_of_product
            q.pop();
            string id = q.front();
            get_quantity_of_product(id);
        }

        else if(q.front()=="get_product_profit"){ //get_product_profit
            q.pop();
            string id = q.front();
            get_product_profit(id);
        }

        else if(q.front()=="get_fewest_product"){ //get_fewest_product
            get_fewest_product();
        }

        else if(q.front()=="get_most_popular_product"){ //get_most_popular_product
            get_most_popular_product();
        }

        else if(q.front()=="get_orders_report"){ //get_orders_report
            get_orders_report();
        }

        else if(q.front()=="export_orders_report"){ //export_orders_report
            q.pop();
            string path = q.front();
            export_orders_report(path);
        }
    
        else if(q.front()=="get_average_price"){ //get_average_price
            q.pop();
            string id = q.front();
            // cout << "Average price of product " << id << ": " << get_average_price(id) << endl;
            cout << get_average_price(id) << endl;

        }
    }

    return 0;
}


