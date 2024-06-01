#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
// G = (V, E) is a graph
// P = physical page
bool all_neighbours_in_P(vector <int> neighbours, vector <vector <int> > P, int b){
    bool has[neighbours.size()];
    for(int i=0;i<neighbours.size();++i)
        has[i] = false;
    for(int i=0;i<P[b].size();++i)
        for(int j=0;j<neighbours.size();++j)
            if(P[b][i]==neighbours[j]){
                has[j] = true;
                break;
            }
    return true;
}
vector <vector <int> > store_all_neighbours_in_P(vector <int> neighbours, vector <vector <int> > P, int b){
    for(int i=0;i<neighbours.size();++i)
        P[b].push_back(neighbours[i]);
    return P;
}
bool GTT_ok(vector <int> GTT, vector <vector <int> > P, int n, vector <vector <int> > G){
	// If all vertices in GTT or in P, return true
    bool ok[n];
    for(int i=0;i<n;++i)
        ok[i] = false;
    for(int i=0;i<GTT.size();++i)
        ok[GTT[i]] = true;
    for(int i=0;i<P.size();++i)
        for(int j=0;j<P[i].size();++j)
            ok[P[i][j]] = true;
    for(int i=0;i<n;++i)
        if(!ok[i])
			return false;
    /*for(int i=0;i<G.size();++i){
        bool cnt = false;
        for(int j=0;j<GTT.size();++j){
            if(GTT[j]==i){
                cnt = true;
                break;
            }
        }
        if(cnt) continue;
        for(int j=0;j<G[i].size();++j){
            cnt = false;
            int x=i, y=G[i][j];
            for(int k=0;k<P[x].size()&&cnt==0;++k)
                if(P[x][k]==y)
                    cnt = true;
            for(int k=0;k<P[y].size()&&cnt==0;++k)
                if(P[y][k]==x)
                    cnt = true;
            if(cnt==0)
                return false;
        }
    }*/
    return true;
}
void pt(vector <int> GTT, vector <vector <int> > P, vector <vector <int>> G){
    int m=0,n=G.size(),m2=0,rp=0;
    set <pair <int,int>> st;
    for(int i=0;i<n;++i){
        m+=G[i].size();
        m2+=P[i].size();
        if(n<10){
        	cout << i << ": ";
        	for(int j=0;j<P[i].size();++j){
        		cout << P[i][j] << " ";
			}
			cout << "\n";
		}
    }
    cout << "GTT size: " << GTT.size() << "\n";
    //for(int i=0;i<GTT.size();++i)
    //    cout << GTT[i]+1 << " ";
    //cout << "Page: \n";
    for(int i=0;i<P.size();++i){
        //cout << i+1 << ": ";
        for(int j=0;j<P[i].size();++j){
            if(st.count({i,P[i][j]})==0){
                st.insert({i,P[i][j]});
                st.insert({P[i][j],i});
            }
            else{
                ++rp;
            }
            for(int k=0;k<GTT.size();++k)
                if(GTT[k]==P[i][j]){
                    ++rp;
                }
            //cout << P[i][j]+1 << " ";
        }
        //cout << "\n";
    }
    cout << "data usage efficiency: ";
    cout << (double)((double)1 - ((double)(GTT.size() + m2) / (double)(n + m))) * 100.0 << "%\n";
    cout << "vertices are stored in GTT: " << (double)GTT.size()/n * 100.0 << "%\n";
    cout << "avg page used: " << (double)m2*2/GTT.size() << "\n"; 
    cout << "Duplicate data rate: " << ((double)(rp+m)/m - 1) * 100.0 << "%\n";
    return;
}
void GTT_initialization_algorithm(vector <vector <int> > G, vector <vector <int> > P){
    vector <int> GTT;
    for(int i=0;i<G.size();++i){
        if(find(GTT.begin(), GTT.end(), i) != GTT.end()) continue;
        bool no_vertex = true;
        // find the minimum vertex that has all its neighbours in P
        for(int j=i;j<G.size();++j){
            if(all_neighbours_in_P(G[j], P, j)){
                GTT.push_back(j);
                no_vertex = false;
                P = store_all_neighbours_in_P(G[j], P, j);
                break;         
            }
        }
        if(no_vertex){
            GTT.push_back(i);
            P = store_all_neighbours_in_P(G[i], P, i);       
        }
        if(GTT_ok(GTT, P, G.size(), G))
            break;
    }
    pt(GTT, P, G);
    return;
}
void GTT_initialization_algorithm_2(vector <vector <int> > G, vector <vector <int> > P){
    // TC: O(n^2), n = |Vertices|
    // SC: O(n)
    vector <int> GTT;
    set <int> GT;
	set <pair <int,int> > st;
    vector <vector <int> > cpG = G;
    int m=0;
    for(int i=0;i<G.size();++i)
        m+=G[i].size();
    while(m){
        for(int i=0;i<G.size();++i){
            if(G[i].size()){
            	int t=-1;
            	for(int j=0;j<G[i].size()&&t==-1;++j){
            		if(st.count({i,G[i][j]})==0)t=j;
				}
				if(t!=-1){
	                //cout << "push " << i << " " << G[i][t] << "\n";
	                if(!GT.count(i)){
	                	GTT.push_back(i); 
	                	GT.insert(i);
	                }
	                if(!GT.count(G[i][t])){
	                	GTT.push_back(G[i][t]);
	                	GT.insert(G[i][t]);
	                }
	                st.insert({i,G[i][t]});
	                st.insert({G[i][t],i});
					for(int j=0;j<G[i].size();++j){
	                    if(G[i][j]!=G[i][t]&&st.count({i,G[i][j]})==0){
	                        P[i].push_back(G[i][j]);
	                    	st.insert({i,G[i][j]});
	                    	st.insert({G[i][j],i});
						}
						GT.insert(G[i][j]); 
	                }
	                int b = G[i][t];
	                for(int j=0;j<G[b].size();++j){
	                    if(G[b][j]!=i&&st.count({b,G[b][j]})==0){
	                        P[b].push_back(G[b][j]);
	                		st.insert({b,G[b][j]});
	                    	st.insert({G[b][j],b});
						}
						GT.insert(G[b][j]);
					}
	                m-=G[i].size();
	                m-=G[b].size();
	                G[i].clear();
	                G[b].clear();
            	}
            }
        }
        if(GTT_ok(GTT, P, G.size(), cpG))break;
    }
    if(GTT_ok(GTT, P, G.size(), cpG)==false)
        cout << "Wrong GTT\n";
    pt(GTT, P, cpG);
    return;
}
void test(){
	vector <vector <int> > G,P;
    vector <int> GTT;
	int n=5;
	for(int i=0;i<n;++i){
		G.push_back(GTT);
		if(i<n-1)G[i].push_back(i+1);
		P.push_back(GTT);
	}
	GTT_initialization_algorithm(G, P);
    GTT_initialization_algorithm_2(G, P);
}
int main(){
	test();
    int n;
    vector <vector <int> > G;
    vector <int> GTT;
    vector <vector <int> > P;
    set <pair <int,int>> has;
    n = 1000;
    vector <int> rd_arr;
    for(int i=0;i<n;++i){
        rd_arr.push_back(i);
        G.push_back(GTT);
    }
    random_shuffle(rd_arr.begin(), rd_arr.end());
    for(int i=0;i<n-1;++i){
        G[rd_arr[i]].push_back(rd_arr[i+1]);
    }
    G[rd_arr[n-1]].push_back(rd_arr[0]);
    for(int i=0;i<50;++i){
        int x = rand()%n;
        int y = rand()%n;
        if(x==y||has.count({x,y}))continue;
        has.insert({x,y});
        has.insert({y,x});
		G[x].push_back(y);
        G[y].push_back(x);
    }
    /*cin >> n;
    for(int i=0;i<n;++i){
        vector <int> row;
        cout << "Enter neighbours of vertex " << i+1 << ":\n";
        cin >> m;
        for(int j=0;j<m;++j){
            int neighbour;
            cin >> neighbour;
            row.push_back(neighbour);
        }
        G.push_back(row);
    }*/
    for(int i=0;i<n;++i){
        vector <int> row;
        P.push_back(row);
    }
    GTT_initialization_algorithm(G, P);
    GTT_initialization_algorithm_2(G, P);
    for(int i=0;i<5000;++i){
        int x = rand()%n;
        int y = rand()%n;
        if(x==y||has.count({x,y}))continue;
        has.insert({x,y});
        has.insert({y,x});
		G[x].push_back(y);
        G[y].push_back(x);
    }
    GTT_initialization_algorithm(G, P);
    GTT_initialization_algorithm_2(G, P);
    return 0;
}
// 5 1 1 2 0 2 2 1 3 2 2 4 1 3 

