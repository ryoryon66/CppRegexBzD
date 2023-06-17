#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <iostream>
#include <cassert>
#include <cstdlib>

#define USE_SIMPLIFICATION true

using namespace std;

// token type
enum TokenType {
    CHAR, // single character
    STAR, // *
    OR, // |
    LPAREN, // (
    RPAREN, // )
    END, // end of input
};

// token class
class Token {
public:
    TokenType type;
    char value;
    Token(TokenType type, char value) : type(type), value(value) {
        // check if the token is valid
        if (type == CHAR && (value < 'a' || value > 'z')){
            cout << "Invalid token" << endl;
            exit(1);
        }

    }
};



vector <Token> tokenize(string s){
    vector <Token> tokens;
    for (int i = 0; i < int(s.size()); i++){
        char c = s[i];
        switch (c){
            case '*':
                tokens.push_back(Token(STAR, c));
                break;
            case '|':
                tokens.push_back(Token(OR, c));
                break;
            case '(':
                tokens.push_back(Token(LPAREN, c));
                break;
            case ')':
                tokens.push_back(Token(RPAREN, c));
                break;
            default:
                tokens.push_back(Token(CHAR, c));
                break;
        }
    }
    tokens.push_back(Token(END, '$'));
    return tokens;
}

// tree node type
enum NodeType {
    CHAR_NODE, // single character
    STAR_NODE, // *
    OR_NODE, // |
    CONCAT_NODE, // concatenation
    EPSILON_NODE, // epsilon
    EMPTY_NODE // empty set denoted by # in the graph
};

// tree node class
class Node {
public:
    NodeType type;
    char value;
    Node *left;
    Node *right;
    Node(NodeType type, char value, Node *left, Node *right) {
        this->type = type;
        this->value = value;
        this->left = left;
        this->right = right;
    }

    Node* copy () {
        Node* new_node = new Node(type, value, NULL, NULL);
        if (left != NULL){
            new_node->left = left->copy();
        }
        if (right != NULL){
            new_node->right = right->copy();
        }
        return new_node;
    }

    void pretty_print (int indent){
        for (int i = 0; i < indent; i++){
            cout << " ";
        }
        switch (type){
            case CHAR_NODE:
                cout << "CHAR_NODE " << value << endl;
                break;
            case STAR_NODE:
                cout << "STAR_NODE " << value << endl;
                left->pretty_print(indent + 2);
                break;
            case OR_NODE:
                cout << "OR_NODE " << value << endl;
                left->pretty_print(indent + 2);
                right->pretty_print(indent + 2);
                break;
            case CONCAT_NODE:
                cout << "CONCAT_NODE " << value << endl;
                left->pretty_print(indent + 2);
                right->pretty_print(indent + 2);
                break;
            case EPSILON_NODE:
                cout << "EPSILON_NODE" << endl;
                break;
            case EMPTY_NODE:
                cout << "EMPTY_NODE" << endl;
                break;
        }
    }

    string to_string(){
        switch (type){
            case CHAR_NODE:
                return string(1, value);
            case STAR_NODE:
                return "(" + left->to_string() + ")" + "*";
            case OR_NODE:
                return "(" + left->to_string() + ")" + "|"  + "(" + right->to_string() + ")";
            case CONCAT_NODE:
                return left->to_string() + right->to_string();
            case EPSILON_NODE:
                return "<epsilon>";
            case EMPTY_NODE:
                return "<empty>";
        }

        cout << "error at to_string" << endl;
        cout << type << endl;
        exit(1);
    }

    string get_id(){
        
        long long id = (long long) this;


        // from this address, get a string composed of alphanumeric characters. let the address as seed
        srand(id);
        string s;
        for (int i = 0; i < 30; i++){
            s += (char) ('a' + rand() % 26);
        }

        return s;
    }

    void to_dot(ostream& out) {
        string label = "unknown";

        if (type == EPSILON_NODE) {
            label = "epsilon";
        } else if (type == EMPTY_NODE) {
            label = "empty";
        } else if (type == STAR_NODE) {
            label = "*";
        } else if (type == OR_NODE) {
            label = "|";
        } else if (type == CONCAT_NODE) {
            label = "concat";
        } else if (type == CHAR_NODE) {
            label = string(1, value);
        }


        out << "  " << get_id() << " [label=\"" << label << "\"];" << endl;
        if (left != NULL) {
            out << "  " << get_id() << " -> " << left->get_id() << ";" << endl;
            left->to_dot(out);
        }
        if (right != NULL) {
            out << "  " << get_id() << " -> " << right->get_id() << ";" << endl;
            right->to_dot(out);
        }
    }

    void to_dot(string filename) {
        ofstream out(filename);
        out << "digraph G {" << endl;
        to_dot(out);
        out << "}" << endl;
        out.close();


        string visualized_file = "visualized_" + filename.substr(0, filename.find_last_of('.')) + ".png";

        // dot -Tpng filename -o visualized_file
        string command = "dot -Tpng " + filename + " -o " + visualized_file;
        system(command.c_str());
    }

};



// expression = concat ( '|' expression )*
// concat = star ( concat )*
// star = star '*' | atom
// atom = char | '(' expression ')'

class Parser {
public:
    vector <Token> tokens;
    int pos;
    Parser(vector <Token> tokens) : tokens(tokens), pos(0) {}
    Node *parse();
    Node *expression();
    Node *concat();
    Node *star();
    Node *atom();
};

Node *Parser::parse(){
    return expression();
}

Node *Parser::expression(){
    Node *left = concat();
    if (tokens[pos].type == OR){
        pos++;
        Node *right = expression();
        return new Node(OR_NODE, '|', left, right);
    }
    return left;
}

Node *Parser::concat(){
    Node *left = star();
    if (tokens[pos].type != OR && tokens[pos].type != RPAREN && tokens[pos].type != END){
        Node *right = concat();
        return new Node(CONCAT_NODE, '.', left, right);
    }
    return left;
}

Node *Parser::star(){
    Node *left = atom();
    if (tokens[pos].type == STAR){
        pos++;
        return new Node(STAR_NODE, '*', left, NULL);
    }
    return left;
}

Node *Parser::atom(){
    if (tokens[pos].type == CHAR){
        Node *node = new Node(CHAR_NODE, tokens[pos].value, NULL, NULL);
        pos++;
        return node;
    }
    if (tokens[pos].type == LPAREN){
        pos++;
        Node *node = expression();
        if (tokens[pos].type != RPAREN){
            cout << "Expected )" << endl;
            exit(1);
        }
        pos++;
        return node;
    }
    cout << "Expected char or (" << endl;
    exit(1);
}

// pattern match using derivatives

Node* simplify(Node* expr){
    switch(expr->type){
        case CHAR_NODE:
            return new Node(CHAR_NODE, expr->value, NULL, NULL);
        case STAR_NODE:
            // epsilon* = epsilon 何も受理しないが0回以上繰り返すのはepsilonのみ受理する
            if (expr->left->type == EPSILON_NODE || expr->left->type == EMPTY_NODE){
                return new Node(EPSILON_NODE, ' ', NULL, NULL);
            } else{
                return new Node(STAR_NODE, '*', simplify(expr->left), NULL);
            }
        case OR_NODE:

            // empty | a = a
            if (expr->left->type == EMPTY_NODE){
                return simplify(expr->right);
            }

            // a | empty = a
            if (expr->right->type == EMPTY_NODE){
                return simplify(expr->left);
            }

            return new Node(OR_NODE, '|', simplify(expr->left), simplify(expr->right));


        case CONCAT_NODE:
            if (expr->left->type == EMPTY_NODE || expr->right->type == EMPTY_NODE){
                return new Node(EMPTY_NODE, ' ', NULL, NULL);
            }

            if (expr->left->type == EPSILON_NODE && expr->right->type == EPSILON_NODE){
                return new Node(EPSILON_NODE, ' ', NULL, NULL);
            } else if (expr->left->type == EPSILON_NODE){
                return simplify(expr->right);
            }else if (expr->right->type == EPSILON_NODE){
                return simplify(expr->left);
            }

            return new Node(CONCAT_NODE, '.', simplify(expr->left), simplify(expr->right));
        case EPSILON_NODE:
            return new Node(EPSILON_NODE, ' ', NULL, NULL);
        case EMPTY_NODE:
            return new Node(EMPTY_NODE, ' ', NULL, NULL);
    }

    cout << "error" << endl;
    exit(1);
}

bool accept_epsilon(Node* reg_expr){
    switch(reg_expr->type){
        case EPSILON_NODE:
            return true;
        case EMPTY_NODE:
            return false;
        case CHAR_NODE:
            return false;
        case STAR_NODE:
            return true;
        case OR_NODE:
            return accept_epsilon(reg_expr->left) || accept_epsilon(reg_expr->right);
        case CONCAT_NODE:
            return accept_epsilon(reg_expr->left) && accept_epsilon(reg_expr->right);
    }

    cout << "error" << endl;
    exit(1);
}

Node* differentiate(Node* expr,char c){
    switch(expr->type){
        case CHAR_NODE:
            if(expr->value == c){
                return new Node(EPSILON_NODE, ' ', NULL, NULL); //　最後の一文字が食われたら空文字列のみを受理する言語になる
            } else{
                return new Node(EMPTY_NODE, '#', NULL, NULL); //　最後の一文字が食われたら空集合を受理する言語になる
            }

        case STAR_NODE:
            return new Node(CONCAT_NODE, '.', differentiate(expr->left, c), new Node(STAR_NODE, '*', expr->left, NULL));
        
        case OR_NODE:
            return new Node(OR_NODE, '|', differentiate(expr->left, c), differentiate(expr->right, c));
        
        case CONCAT_NODE://可視化を面白くするために永続ではなく短命を採用。
            if(accept_epsilon(expr->left)){
                Node* tmp1 = new Node(CONCAT_NODE, '.', differentiate(expr->left, c), expr->right->copy());
                Node* tmp2 = differentiate(expr->right, c);
                return new Node(OR_NODE, '|', tmp1, tmp2);
            } else{
                return new Node(CONCAT_NODE, '.', differentiate(expr->left, c), expr->right->copy());
            }

        case EPSILON_NODE:
            return new Node(EMPTY_NODE, '#', NULL, NULL);
        
        case EMPTY_NODE:
            return new Node(EMPTY_NODE, '#', NULL, NULL);

        default:
            cout << "error" << endl;
            exit(1);
    }
}

bool match(Node* reg_expr, string s){
    
    for (int i = 0; i < int( s.size() ); i++){
        reg_expr = differentiate(reg_expr, s[i]);
        if (USE_SIMPLIFICATION) {
            reg_expr = simplify(reg_expr);
        }
    }

    return accept_epsilon(reg_expr);
}

bool match_verbose(Node* reg_expr,string s){
    // save to file. 
    if(USE_SIMPLIFICATION) {
        reg_expr = simplify(reg_expr);
    }
    string filename = "graph" + to_string(s.size()) + s + ".dot";
    if(USE_SIMPLIFICATION) filename = "graph" + to_string(s.size()) + s + "_simplified.dot";
    reg_expr->to_dot(filename);
    if(s.size() == 0){
        return accept_epsilon(reg_expr);
    } else{
        cout << "reg_expr: " << reg_expr->to_string() << endl;
        cout << "s: " << s << endl;
        cout << "differentiate: reg_expr by... "  + string(1,s[0])  << endl;
        cout << endl;
        reg_expr = differentiate(reg_expr, s[0]);
        if (USE_SIMPLIFICATION) {
                reg_expr = simplify(reg_expr);
        }
        return match_verbose(reg_expr, s.substr(1));
    }
}


void test_match(){

    string s;
    s = "(a*b)|(ab*)";
    vector <Token> tokens = tokenize(s);
    Parser parser(tokens);
    Node *root = parser.parse();


    assert (match(root, "ab") == true);
    assert (match(root, "aab") == true);
    assert (match(root, "b") == true);
    assert (match(root, "a") == true);
    assert (match(root, "aa") == false);

    assert (match(root, "ba") == false);
    assert (match(root, "bb") == false);
    assert (match(root, "aba") == false);
    assert (match(root, "baa") == false);

    // another test
    s = "(a|b)*abb";

    tokens = tokenize(s);
    parser = Parser(tokens);
    root = parser.parse();
    
    assert (match(root, "abb") == true);
    assert (match(root, "aabb") == true);
    assert (match(root, "babb") == true);
    assert (match(root, "aaabb") == true);
    assert (match(root, "ababb") == true);
    assert (match(root, "bbabb") == true);
    assert (match(root, "aaaabb") == true);
    assert (match(root, "abababaaabb") == true);
    assert (match(root, "abababababb") == true);
    assert (match(root, "ababababababb") == true);
    assert (match(root, "abababababababb") == true);
    assert (match(root, "ababababababababb") == true);


    assert (match(root, "ab") == false);
    assert (match(root, "a") == false);
    assert (match(root, "b") == false);
    assert (match(root, "aa") == false);
    assert (match(root, "ba") == false);
    assert (match(root, "bb") == false);
    assert (match(root, "aba") == false);
    assert (match(root, "baa") == false);
    assert (match(root, "abaa") == false);
    assert (match(root, "baba") == false);

    cout << "test_match passed" << endl;

    return;
}


int main (int argc, char** argv){
    string reg;
    string s;


    if (argc == 4){
        reg = argv[1];
        s = argv[2];
        bool remove_dot =  (*argv[3] == 'd');

        if (remove_dot){ system("rm -f *.dot"); system("rm -f *.png"); }
    } else if (argc == 3){
        reg = argv[1];
        s = argv[2];
    } else if (argc == 2){
        reg = argv[1];
        cout << "string:"; cin >> s;
    } else {
        cout << "usage: ./main [regular_expression] [string] [d if remove dot files]" << endl;
        exit(1);
    }



    vector <Token> tokens = tokenize(reg);
    Parser parser(tokens);
    Node *root = parser.parse();
    
    cout << "abstract syntax tree:" << endl;
    root->pretty_print(0);

    cout << "given_regular_expression: " << root->to_string() << endl;


    match_verbose(root, s);


    bool ans = match(root, s);
    cout << (ans ? "YES" : "NO") << endl;


    // test_match();

    // concatinate png and make gif 端が切れないように大き目に
    // system("convert -delay 100 -loop 0 *.png result.gif");


    
}