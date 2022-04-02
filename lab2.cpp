#include <bits/stdc++.h>

using namespace std;

int global_depth;
int bucket_capacity;
class Bucket{
    public:
    vector<int> bucket;
    int capacity;
    int local_depth;
    int last_index;

    Bucket(int x){
        capacity = x;
        bucket.resize(x);
        last_index = 0;
        local_depth = global_depth;
    }

    bool insert(int x){
        if(last_index == capacity){
            return false;
        }
        bucket[last_index] = x;
        last_index++;
        return true;
    }

    int search(int x){
        for(int i=0; i<last_index; i++){
            if(bucket[i] == x){
                return i;
            }
        }
        return -1;
    }

    void remove(int x){
        int ind = this->search(x);
        if(ind == -1) return;
        bucket[ind] = bucket[last_index-1];
        last_index--;
    }

    void pop_back(){
        if(last_index == 0) return;
        last_index--;
    }

    void clear(){
        last_index=0;
    }
};

vector<Bucket *> directory;
vector<Bucket *> bucket_creation_list;

void create_buckets(){
    directory.resize(1 << (global_depth));
    for(int i=0; i<directory.size(); i++){
        directory[i] = new Bucket(bucket_capacity);
        bucket_creation_list.push_back(directory[i]);
    }
}

int get_index(int y){
    int x = (directory.size() - 1);
    return (x & y);
}

void fix_bucket(int index){
    Bucket * bk = new Bucket(bucket_capacity);
    bucket_creation_list.push_back(bk);
    int n = directory.size()/2;
    directory[index+n] = bk;
    auto b = directory[index];
    vector<int> tmp(bucket_capacity);
    for(int i=0; i<bucket_capacity; i++){
        tmp[i] = b->bucket[i];
    }
    b->local_depth++;
    b->clear();
    bk->local_depth = b->local_depth;
    for(int i=0; i<bucket_capacity; i++){
        // cout << "tmp: " << tmp[i] << endl;
        int ind = get_index(tmp[i]);
        // cout << "tmp_i: " << ind << endl;
        auto bt = directory[ind];
        bt->insert(tmp[i]);
    }
}

void double_directory(int bucket_index){
    if(global_depth == 20) return;
    global_depth++;
    int n = directory.size();
    directory.resize((1 << global_depth));
    for(int i=0; i<n; i++){
        if(i == bucket_index){
            fix_bucket(i);
            continue;
        }
        directory[i+n] = directory[i];
    }
}

bool insert(int x){
    int ind = get_index(x);
    auto b = directory[ind];
    if(b->insert(x)){
        return true;
    }
    if(global_depth == 20) return false;
    if(b->local_depth == global_depth){
        double_directory(ind);
    }else{
        fix_bucket(ind);
    }
    return insert(x);
}

bool search(int x){
    int ind = get_index(x);
    auto b = directory[ind];
    int i = b->search(x);
    if(i == -1) return false;
    return true;
}

void delete_val(int x){
    int ind = get_index(x);
    auto b = directory[ind];
    b->remove(x);
    return;
}

void display_table(){
    cout << global_depth << endl;
    cout << bucket_creation_list.size() << endl;
    for(auto b: bucket_creation_list){
        // cout << b->last_index << " " << b->local_depth << endl;
        cout << "Size: " << b->last_index << ", Local Depth: " << b->local_depth << endl;

    }
}

void display_buckets(){
    cout << "Bucket List: " << endl;
    for(auto b: bucket_creation_list){
        // cout << b->last_index << " " << b->local_depth << endl;
        // cout << "Size: " << b->last_index << ", Local Depth: " << b->local_depth << endl;
        
        for(auto c: b->bucket){
            cout << c << " ";
        }
        cout << endl;
    }
}
void take_input(){
    cin >> global_depth >> bucket_capacity;
    create_buckets();
    int d;
    cin >> d;
    while(d != 6){
        display_buckets();
        cout << "Directory Size: " << directory.size() << endl;
        if(d == 2){
            int x;
            cin >> x;
            insert(x);
        }else if(d == 3){
            int x;
            cin >> x;
            search(x);
        }
        else if(d == 4){
            int x;
            cin >> x;
            delete_val(x);
        }
        else if(d == 5){
            display_table();
        }
        cin >> d;
    }
}

int main(){
    take_input();
    display_buckets();
}