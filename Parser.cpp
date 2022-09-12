#include "Parser.h"

Parser::Parser(std::vector<Token>& tokens) {
    if (tokens.size() <= 1) {
        throw "No tokens passed to Parser.";
    }
    this->tokens = &tokens;
    this->index = 0;
    this->curTok = tokens.at(0);
}

bool Parser::hasNext(int steps = 1) {
    return (index + steps) < (int) tokens->size();
}

void Parser::getNext() {
    if (hasNext()) {
        index++;
        curTok = tokens->at(index);
    } else {
        curTok = Token::getNullToken();
    }
}

Token Parser::lookAhead(int steps = 1) {
    return hasNext(steps) ? tokens->at(index + steps) : Token::getNullToken();
}

shared_ptr<AstNode> Parser::parse() {
    return comp_expr1();
}

shared_ptr<AstNode> Parser::expr() {
    if (curTok.type == OP && curTok.value == "!") {
        Token opTok = curTok;
        getNext();

        auto node = expr();
        if (node == nullptr) {
            throw "Expected expr after op tok";
        }
        return shared_ptr<AstNode>(new UnaryOpNode(opTok, node));
    }

    return comp_expr1();
}

shared_ptr<AstNode> Parser::comp_expr1() {
    auto left = comp_expr2();
    std::unordered_map<std::string, bool> ops = {{"&&", true}, {"||", true}};

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

shared_ptr<AstNode> Parser::comp_expr2() {
    auto left = arith_expr();
    std::unordered_map<std::string, bool> ops = {{"<", true}, {">", true}, {"<=", true}, {">=", true},
        {"==", true}, {"!=", true}};

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


// Arithmetic parsing working
shared_ptr<AstNode> Parser::arith_expr() {
    auto left = term();
    std::unordered_map<std::string, bool> ops = {{"+", true}, {"-", true}};

    while (curTok.type == OP && (ops.find(curTok.value) != ops.end())) {
        Token opTok = curTok;
        getNext();

        auto right = term();
        if (right == nullptr) {
            throw "Expected expr after op tok";
        }
        left = shared_ptr<AstNode>(new BinOpNode(left, opTok, right));
    }

    return left;
}

shared_ptr<AstNode> Parser::term() {
    auto left = power();
    std::unordered_map<std::string, bool> ops = {{"*", true}, {"/", true}, {"%", true}};

    while (curTok.type == OP && (ops.find(curTok.value) != ops.end())) {
        Token opTok = curTok;
        getNext();

        auto right = power();
        if (right == nullptr) {
            throw "Expected expr after op tok";
        }
        left = shared_ptr<AstNode>(new BinOpNode(left, opTok, right));
    }

    return left;
}

shared_ptr<AstNode> Parser::power() {
    auto left = atom();

    while (curTok.type == OP && curTok.value == "^") {
        Token opTok = curTok;
        getNext();

        auto right = power();
        if (right == nullptr) {
            throw "Expected expr after op tok";
        }
        left = shared_ptr<AstNode>(new BinOpNode(left, opTok, right));
    }

    return left;
}

shared_ptr<AstNode> Parser::atom() {
    Token tok = curTok;

    if (tok.type == OP && (tok.value == "+" || tok.value == "-")) {
        getNext();
        auto node = atom();
        if (node == nullptr) {
            throw "Expected atom after unary operator.";
        }
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
    } else if (tok.type == LPAREN) {
        getNext();
        auto node = arith_expr();
        if (node == nullptr) {
            throw "Expected expr after parenthesis";
        }
        if (curTok.type != RPAREN) {
            throw "Expected ')'";
        }
        getNext();
        return shared_ptr<AstNode>(new StringNode(tok));
    }

    throw "no atom";

    return nullptr;
}
