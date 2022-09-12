#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "Token.h"
#include "AstNodes.h"
#include <vector>
#include <unordered_map>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class Parser {
    public:
        std::vector<Token> *tokens;
        int index = 0;
        Token curTok;

        Parser(std::vector<Token> tokens) {
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
                curTok = NULLTOK;
            }
        }

        Token lookAhead(int steps = 1) {
            return hasNext(steps) ? tokens->at(index + steps) : NULLTOK;
        }

        AstNode* parse() {
            return expr();
        }

        AstNode* expr() {
            auto left = atom();
            std::unordered_map<std::string, bool> ops = {{"+", true}, {"-", true}};

            while (curTok.type == OP && (ops.find(curTok.value) != ops.end())) {
                Token opTok = curTok;
                getNext();

                auto right = expr();
                if (right == nullptr) {
                    throw "Expected expr after op tok";
                }
                left = new BinOpNode(left, opTok, right);
            }

            return left;
        }

        AstNode* atom() {
            Token tok = curTok;

            if (tok.type == OP && (tok.value == "-" || tok.value == "-")) {
                getNext();
                AstNode* node = atom();
                return new UnaryOpNode(tok, node);
            } else if (tok.type == INT) {
                getNext();
                return new NumberNode(tok);
            } else if (tok.type == DOUBLE) {
                getNext();
                return new NumberNode(tok);
            } else if (tok.type == STRING) {
                getNext();
                return new StringNode(tok);
            } else {
                throw "No matching atom";
                return nullptr;
            }
        }

};

#endif // PARSER_H_INCLUDED
