#include <list>
#include <iostream>

using namespace std;




class coord {
public:
    
    int x, y;
    coord( int a = 0, int b = 0 ) {
        x = a; y = b;
    }
    
    bool operator==( const coord& o ) {
        return o.x == x && o.y == y;
    }
    
    coord operator +( const coord& o ) {
        return coord( o.x + x, o.y + y ); }
};

class gameMap {
public:
    char m[8][8];
    int width, height;
    gameMap() {
        width=8;
        height=8;
        char x[8][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 1, 0},
            {0, 0, 1, 0, 0, 0, 1, 0},
            {0, 0, 1, 0, 0, 0, 1, 0},
            {0, 0, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0}
        };
        for( int r = 0; r < 8; r++ )
            for( int s = 0; s < 8; s++ )
                m[s][r] = x[r][s];
    }
    
    int get( int x, int y ){
        return m[x][y];
    }
};

class coorNod {
public:
    
    coord position, father;
    int gap, value;
    
    //Overloading operator that compares Coord with coorNod
    bool operator== (const coord& o ){
        return position == o;
    }
};

class algorithm {
public:
    
    gameMap m; coord end, start;
    coord neighbours[8];
    list<coorNod> open;
    list<coorNod> closed;
    
    algorithm() {
        neighbours[0] = coord( -1, -1 );
        neighbours[1] = coord(  1, -1 );
        neighbours[2] = coord( -1,  1 );
        neighbours[3] = coord(  1,  1 );
        neighbours[4] = coord(  0, -1 );
        neighbours[5] = coord( -1,  0 );
        neighbours[6] = coord(  0,  1 );
        neighbours[7] = coord(  1,  0 );
    }
    
    int gapCost( coord& p ){
        //Need better heuristic
        int x = end.x - p.x, y = end.y - p.y;
        return( x * x + y * y );
    }
    
    bool isGood( coord& p ) {
        return ( p.x >-1 && p.y > -1 && p.x < m.width && p.y < m.height );
    }
    
    bool existcoord( coord& p, int value ) {
        list<coorNod>::iterator i;
        i = find( closed.begin(), closed.end(), p );
        if( i != closed.end() ) {
            if( ( *i ).value + ( *i ).gap < value )
                return true;
            else { closed.erase( i ); return false;
            }
        }
        i = find( open.begin(), open.end(), p );
        if( i != open.end() ) {
            if( ( *i ).value + ( *i ).gap < value ) return true;
            else { open.erase( i ); return false; }
        }
        return false;
    }
    
    bool checkN( coorNod& n ) {
        int stepvalue, nc, gap;
        coord neighbour;
        
        for( int x = 0; x < 8; x++ ) {
            // one can make diagonals have different value
            stepvalue = 1;
            neighbour = n.position + neighbours[x];
            if( neighbour == end ) return true;
            
            if( isGood( neighbour ) && m.get( neighbour.x, neighbour.y ) != 1 ) {
                nc = stepvalue + n.value;
                gap = gapCost( neighbour );
                if( !existcoord( neighbour, nc + gap ) ) {
                    coorNod m;
                    m.value = nc; m.gap = gap;
                    m.position = neighbour;
                    m.father = n.position;
                    open.push_back( m );
                }
            }
        }
        return false;
    }
    
    bool search( coord& s, coord& e, gameMap& mp ) {
        coorNod n; end = e; start = s; m = mp;
        n.value = 0; n.position = s; n.father = 0; n.gap = gapCost( s );
        open.push_back( n );
        while( !open.empty() ) {
            //open.sort();
            coorNod n = open.front();
            open.pop_front();
            closed.push_back( n );
            if( checkN( n ) ) return true;
        }
        return false;
    }
    
    
    
    
    int pathway( list<coord>& pathway ) {
        pathway.push_front( end );
        int value = 1 + closed.back().value;
        pathway.push_front( closed.back().position );
        coord father = closed.back().father;
        
        for( int y = -1; y < 9; y++ ) {
            for( int x = -1; x < 9; x++ ) {
                if( x < 0 || y < 0 || x > 7 || y > 7 || m.get( x, y ) == 1 )
                    cout << char(77);
                else {
                    if( find( pathway.begin(), pathway.end(), coord( x, y ) )!= pathway.end() )
                        cout << "*";
                    else cout << " ";
                }
            }
            cout << "\n";
        }
        
        cout << "\n";
        
        for( list<coorNod>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++ ) {
            if( ( *i ).position == father && !( ( *i ).position == start ) ) {
                pathway.push_front( ( *i ).position );
                father = ( *i ).father;
                
                for( int y = -1; y < 9; y++ ) {
                    for( int x = -1; x < 9; x++ ) {
                        if( x < 0 || y < 0 || x > 7 || y > 7 || m.get( x, y ) == 1 )
                            cout << char(77);
                        else {
                            if( find( pathway.begin(), pathway.end(), coord( x, y ) )!= pathway.end() )
                                cout << "*";
                            else cout << " ";
                        }
                    }
                    cout << "\n";
                }
                
                cout << "\n";
                
            }
            
        }
        pathway.push_front( start );
        
        return value;
    }
    
};

int main() {
    gameMap m;
    coord s, e( 7, 7);
    algorithm as;
    
    if( as.search( s, e, m ) ) {
        list<coord> pathway;
        //This is where everything happens
        int c = as.pathway( pathway );
        for( int y = -1; y < 9; y++ ) {
            for( int x = -1; x < 9; x++ ) {
                if( x < 0 || y < 0 || x > 7 || y > 7 || m.get( x, y ) == 1 )
                    cout << char(77);
                else {
                    if( find( pathway.begin(), pathway.end(), coord( x, y ) )!= pathway.end() )
                        cout << "*";
                    else cout << " ";
                }
            }
            cout << "\n";
        }
        
        cout << "\npathway value " << c << ": ";
        for( list<coord>::iterator i = pathway.begin(); i != pathway.end(); i++ ) {
            cout<< "(" << ( *i ).x << ", " << ( *i ).y << ") ";
        }
    }
    cout << "\n\n";
    return 0;
}
