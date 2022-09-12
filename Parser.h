#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Token.h"
#include "AstNodes.h"
#include <vector>
#include <unordered_map>
#include <memory>

using std::unique_ptr;
using std::shared_ptr;

class Parser {
    public:
        std::vector<Token> *tokens;
        int index = 0;
        Token curTok;

        Parser(std::vector<Token>& tokens) {
            if (tokens.size() == 1) {
                throw "No tokens passed to Parser.";
            }
            this->tokens = &tokens;
            this->curTok = tokens.at(0);
        }

        bool hasNext(int steps = 1) {
            return (index + steps) < (int) tokens->size();
        }

        void getNext() {
            if (hasNext()) {
                index++;
                curTok = tokens->at(index);
            } else {
                curTok = Token::getNullToken();
            }
        }

        Token lookAhead(int steps = 1) {
            return hasNext(steps) ? tokens->at(index + steps) : Token::getNullToken();
        }

        shared_ptr<AstNode> parse() {
            return expr();
        }

        shared_ptr<AstNode> expr() {
            auto left = atom();
            std::unordered_map<std::string, bool> ops = {{"+", true}, {"-", true}};

            while (curTok.type == OP && (ops.find(curTok.value) != ops.end())) {
                Token opTok = curTok;
                getNext();

                auto right = expr();
                if (right == nullptr) {
                    throw "Expected expr after op tok";
                }
                left = shared_ptr<AstNode>(new BinOpNode(left, opTok, right));
            }

            return left;
        }

        shared_ptr<AstNode> atom() {
            Token tok = curTok;

            if (tok.type == OP && (tok.value == "+" || tok.value == "-")) {
                getNext();
                auto node = atom();
                return shared_ptr<AstNode>(new UnaryOpNode(tok, node));
            } else if (tok.type == INT) {
                getNext();
                return shared_ptr<AstNode>(new NumberNode(tok));
            } else if (tok.type == DOUBLE) {
                getNext();
                return shared_ptr<AstNode>(new NumberNode(tok));
            } else if (tok.type == STRING) {
                getNext();
                return shared_ptr<AstNode>(new StringNode(tok));
            } else {
                throw "No matching atom";
                return nullptr;
            }
        }

};

#endif // PARSER_H_INCLUDED
