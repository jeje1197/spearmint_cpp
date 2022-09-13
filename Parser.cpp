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

void Parser::skipSemis() {
    while (curTok.type == SEMICOLON) {
        getNext();
    }
}

AstNode Parser::parse() {
    return statement();
}

std::vector<AstNode> Parser::statements() {
    std::vector<AstNode> statements;


    return statements;
}

AstNode Parser::statement() {
    return expr();
}

// Variable Action Parsing
AstNode Parser::varDeclaration() {
    return expr();
}

AstNode Parser::varAssign() {
    return expr();
}

// Expression Parsing
AstNode Parser::expr() {
    return binOp(&not_expr, {"&&", "||"}, &not_expr);
}

AstNode Parser::not_expr() {
    if (curTok.type == OP && curTok.value == "!") {
        Token opTok = curTok;
        getNext();

        auto node = comp_expr();
        if (node == nullptr) {
            throw "Expected expr after op tok";
        }
        return AstNode(new UnaryOpNode(opTok, node));
    }

    return comp_expr();
}

AstNode Parser::comp_expr() {
    return binOp(&arith_expr, {"<", ">", "<=", ">=", "==", "!="}, &arith_expr);
}

// Arithmetic parsing
AstNode Parser::arith_expr() {
    return binOp(&term, {"+", "-"}, &term);
}

AstNode Parser::term() {
    return binOp(&power, {"*", "/", "%"}, &power);
}

AstNode Parser::power() {
    return binOp(&atom, {"^"}, &power);
}

AstNode Parser::atom() {
    Token tok = curTok;
    std::set<std::string> unaryOps = {"+", "-"};


    if (tok.matches(OP, unaryOps)) {
        getNext();
        auto node = atom();
        if (node == nullptr) {
            throw "Expected atom after unary operator.";
        }
        return AstNode(new UnaryOpNode(tok, node));
    } else if (tok.matches(INT)) {
        getNext();
        return AstNode(new NumberNode(tok));
    } else if (tok.matches(DOUBLE)) {
        getNext();
        return AstNode(new NumberNode(tok));
    } else if (tok.matches(STRING)) {
        getNext();
        return AstNode(new StringNode(tok));
    } else if (tok.matches(ID)) {
        getNext();
        return AstNode(new VarAccessNode(tok));
    } else if (tok.matches(LPAREN)) {
        getNext();
        auto node = arith_expr();
        if (node == nullptr) {
            throw "Expected expr after parenthesis";
        }

        if (!curTok.matches(RPAREN)) {
            throw "Expected ')'";
        }
        getNext();
        return AstNode(new StringNode(tok));
    }

    throw "No atom found";
    return nullptr;
}

AstNode Parser::binOp(ParserFunction func1, std::set<std::string> ops, ParserFunction func2) {
    auto left = ((*this).*func1)();

    while (curTok.matches(OP, ops)) {
        Token opTok = curTok;
        getNext();

        auto right = ((*this).*func2)();
        if (right == nullptr) {
            throw "Expected expr after op tok";
        }
        left = AstNode(new BinOpNode(left, opTok, right));
    }

    return left;
}


