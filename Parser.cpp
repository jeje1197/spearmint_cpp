#include "Parser.h"

Parser::Parser(std::vector<Token>& tokens) {
    if (tokens.size() <= 1) {
        throw Exception("No tokens passed to Parser.");
    }
    this->tokens = &tokens;
    this->index = 0;
    this->curTok = tokens.at(0);
}

bool Parser::hasNext(int steps = 1) {
    return (index + steps) < (int)tokens->size();
}

void Parser::getNext() {
    if (hasNext()) {
        index++;
        curTok = tokens->at(index);
    }
    else {
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

std::vector<AstNode> Parser::parse() {
    return statements(END);
}

std::vector<AstNode> Parser::statements(int ENDTYPE) {
    std::vector<AstNode> statements;

    skipSemis();
    while (!curTok.matches(ENDTYPE) || !curTok.matches(END)) {
        AstNode statement_node = statement();
        if (statement_node == nullptr) break;

        if (!curTok.matches(SEMICOLON)) {
            throw Exception("Expected semicolon after statement.");
        }
        statements.push_back(statement_node);
        skipSemis();
    }

    return statements;
}

AstNode Parser::statement() {
    if (curTok.matches(KEYWORD, "var")) {
        return varDeclaration();
    }
    else if (curTok.matches(ID) && lookAhead().matches(OP, "=")) {
        return varAssign();
    }
    else if (curTok.matches(KEYWORD, "if")) {
        return ifStatement();
    }
    else if (curTok.matches(KEYWORD, "for")) {
        return forStatement();
    }
    else if (curTok.matches(KEYWORD, "while")) {
        return whileStatement();
    }
    else if (curTok.matches(KEYWORD, "fun")) {
        return functionDef();
    }

    return expr(); // Can return nullptr
}

AstNode Parser::varDeclaration() {
    if (!curTok.matches(KEYWORD, "var")) {
        throw Exception("Expected keyword var");
    }
    getNext();

    if (!curTok.matches(ID)) {
        throw Exception("Expected identifier");
    }
    Token varNameTok = curTok;
    getNext();

    if (!curTok.matches(OP, "=")) {
        throw Exception("Expected '='");
    }
    getNext();

    AstNode expr_node = expr();
    if (expr_node == nullptr) {
        throw Exception("Expected expression");
    }

    return AstNode(new VarDeclarationNode(varNameTok, expr_node));
}

AstNode Parser::varAssign() {
    if (!curTok.matches(ID)) {
        throw Exception("Expected identifier");
    }
    Token varNameTok = curTok;
    getNext();

    if (!curTok.matches(OP, "=")) {
        throw Exception("Expected '='");
    }
    getNext();

    AstNode expr_node = expr();
    if (expr_node == nullptr) {
        throw Exception("Expected expression");
    }

    return AstNode(new VarAssignNode(varNameTok, expr_node));
}

AstNode Parser::ifStatement() {
    std::vector<AstNode> caseConditions;
    std::vector<std::vector<AstNode>> caseStatements,
        elseCaseStatements;

    // if
    if (!curTok.matches(KEYWORD, "if")) {
        throw Exception("Expected keyword 'if'");
    }
    getNext();

    if (!curTok.matches(LPAREN)) {
        throw Exception("Expected '('");
    }
    getNext();

    AstNode cond_node = expr();
    if (cond_node == nullptr) {
        throw Exception("Expected conditional expression");
    }

    caseConditions.push_back(cond_node);

    if (!curTok.matches(RPAREN)) {
        throw Exception("Expected ')'");
    }
    getNext();

    if (!curTok.matches(LBRACE)) {
        throw Exception("Expected '{'");
    }
    getNext();

    caseStatements.push_back(statements(RBRACE));

    if (!curTok.matches(RBRACE)) {
        throw Exception("Expected '}'");
    }
    getNext();


    // else if
    while (curTok.matches(KEYWORD, "else") && lookAhead().matches(KEYWORD, "if")) {
        getNext();
        getNext();

        if (!curTok.matches(LPAREN)) {
            throw Exception("Expected '('");
        }
        getNext();

        cond_node = expr();
        if (cond_node == nullptr) {
            throw Exception("Expected conditional expression");
        }

        caseConditions.push_back(cond_node);

        if (!curTok.matches(RPAREN)) {
            throw Exception("Expected ')'");
        }
        getNext();

        if (!curTok.matches(LBRACE)) {
            throw Exception("Expected '{'");
        }
        getNext();

        caseStatements.push_back(statements(RBRACE));

        if (!curTok.matches(RBRACE)) {
            throw Exception("Expected '}'");
        }
        getNext();
    }


    // else
    if (curTok.matches(KEYWORD, "else")) {
        getNext();

        if (!curTok.matches(LBRACE)) {
            throw Exception("Expected '{'");
        }
        getNext();

        elseCaseStatements.push_back(statements(RBRACE));

        if (!curTok.matches(RBRACE)) {
            throw Exception("Expected '}'");
        }
        getNext();
    }

    return AstNode(new IfNode(caseConditions, caseStatements, elseCaseStatements));
}

AstNode Parser::forStatement() {
    if (!curTok.matches(KEYWORD, "for")) {
        throw Exception("Expected keyword 'for'");
    }
    getNext();

    if (!curTok.matches(LPAREN)) {
        throw Exception("Expected '('");
    }
    getNext();

    AstNode init_statement = statement();
    if (init_statement == nullptr) {
        throw Exception("Expected  statement");
    }

    if (!curTok.matches(SEMICOLON)) {
        throw Exception("Expected ';'");
    }
    getNext();

    AstNode cond_node = expr();
    if (cond_node == nullptr) {
        throw Exception("Expected conditional expression");
    }

    if (!curTok.matches(SEMICOLON)) {
        throw Exception("Expected ';'");
    }
    getNext();

    AstNode update_statement = statement();
    if (update_statement == nullptr) {
        throw Exception("Expected  statement");
    }

    if (!curTok.matches(RPAREN)) {
        throw Exception("Expected ')'");
    }
    getNext();


    if (!curTok.matches(LBRACE)) {
        throw Exception("Expected '{'");
    }
    getNext();

    std::vector<AstNode> statement_list = statements(RBRACE);

    if (!curTok.matches(RBRACE)) {
        throw Exception("Expected '}'");
    }
    getNext();

    return AstNode(new ForNode(init_statement, cond_node, update_statement, statement_list));
}

AstNode Parser::whileStatement() {
    if (!curTok.matches(KEYWORD, "while")) {
        throw Exception("Expected keyword 'while'");
    }
    getNext();

    if (!curTok.matches(LPAREN)) {
        throw Exception("Expected '('");
    }
    getNext();

    AstNode cond_node = expr();
    if (cond_node == nullptr) {
        throw Exception("Expected conditional expression");
    }

    if (!curTok.matches(RPAREN)) {
        throw Exception("Expected ')'");
    }
    getNext();

    if (!curTok.matches(LBRACE)) {
        throw Exception("Expected '{'");
    }
    getNext();

    std::vector<AstNode> statement_list = statements(RBRACE);

    if (!curTok.matches(RBRACE)) {
        throw Exception("Expected '}'");
    }
    getNext();

    return AstNode(new WhileNode(cond_node, statement_list));
}

AstNode Parser::functionDef() {
    if (!curTok.matches(KEYWORD, "fun")) {
        throw Exception("Expected keyword 'fun'");
    }
    getNext();

    if (!curTok.matches(ID)) {
        throw Exception("Expected identifier");
    }
    Token functionNameTok = curTok;
    getNext();

    std::vector<std::string> argNames;
    if (!curTok.matches(LPAREN)) {
        throw Exception("Expected '('");
    }
    getNext();

    if (curTok.matches(ID)) {
        argNames.push_back(curTok.value);
        getNext();

        while (curTok.matches(COMMA)) {
            getNext();

            if (!curTok.matches(ID)) {
                throw Exception("Expected identifier after ','");
            }
            argNames.push_back(curTok.value);
            getNext();
        }
    }

    if (!curTok.matches(RPAREN)) {
        throw Exception("Expected ')'");
    }
    getNext();

    if (!curTok.matches(LBRACE)) {
        throw Exception("Expected '{'");
    }
    getNext();

    std::vector<AstNode> statement_list = statements(RBRACE);

    if (!curTok.matches(RBRACE)) {
        throw Exception("Expected '}'");
    }
    getNext();

    return AstNode(new FunctionDefNode(functionNameTok, argNames, statement_list));
}


AstNode Parser::classDef() {
    if (!curTok.matches(KEYWORD, "class")) {
        throw Exception("Expected keyword 'class'");
    }
    getNext();

    if (!curTok.matches(ID)) {
        throw Exception("Expected class name identifier");
    }
    Token classNameTok = curTok;
    getNext();

    if (!curTok.matches(LBRACE)) {
        throw Exception("Expected '{'");
    }
    getNext();

    std::vector<AstNode> classStatements = statements(RBRACE);

    if (!curTok.matches(RBRACE)) {
        throw Exception("Expected '}'");
    }
    getNext();

    return AstNode(new ClassDefNode(classNameTok, classStatements));
}


// Expression Parsing
AstNode Parser::expr() {
    return binOp(&Parser::not_expr, { "&&", "||" }, &Parser::not_expr);
}

AstNode Parser::not_expr() {
    if (curTok.type == OP && curTok.value == "!") {
        Token opTok = curTok;
        getNext();

        auto node = comp_expr();
        if (node == nullptr) {
            throw Exception("Expected expr after op tok");
        }
        return AstNode(new UnaryOpNode(opTok, node));
    }

    return comp_expr();
}

AstNode Parser::comp_expr() {
    return binOp(&Parser::arith_expr, { "<", ">", "<=", ">=", "==", "!=" }, &Parser::arith_expr);
}

// Arithmetic parsing
AstNode Parser::arith_expr() {
    return binOp(&Parser::term, {"+", "-"}, &Parser::term);
}

AstNode Parser::term() {
    return binOp(&Parser::power, { "*", "/", "%" }, &Parser::power);
}

AstNode Parser::power() {
    return binOp(&Parser::call, { "^" }, &Parser::power);
}

AstNode Parser::call() {
    AstNode atom_node = atom();
   
    return atom_node;
}

AstNode Parser::atom() {
    Token tok = curTok;
    std::set<std::string> unaryOps = { "+", "-" };


    if (tok.matches(OP, unaryOps)) {
        getNext();
        auto node = atom();
        if (node == nullptr) {
            throw Exception("Expected atom after unary operator");
        }
        return AstNode(new UnaryOpNode(tok, node));
    }
    else if (tok.matches(INT)) {
        getNext();
        return AstNode(new NumberNode(tok));
    }
    else if (tok.matches(DOUBLE)) {
        getNext();
        return AstNode(new NumberNode(tok));
    }
    else if (tok.matches(STRING)) {
        getNext();
        return AstNode(new StringNode(tok));
    }
    else if (tok.matches(ID)) {
        getNext();
        return AstNode(new VarAccessNode(tok));
    }
    else if (tok.matches(LPAREN)) {
        getNext();
        AstNode node = expr();
        if (node == nullptr) {
            throw Exception("Expected expr after parenthesis");
        }

        if (!curTok.matches(RPAREN)) {
            throw Exception("Expected ')'");
        }
        getNext();
        return node;
    }

    return nullptr;
}

AstNode Parser::binOp(ParserFunction func1, std::set<std::string> ops, ParserFunction func2) {
    AstNode left = ((*this).*func1)();

    while (curTok.matches(OP, ops)) {
        Token opTok = curTok;
        getNext();

        AstNode right = ((*this).*func2)();
        if (right == nullptr) {
            throw Exception("Expected expr after operator");
        }
        left = AstNode(new BinOpNode(left, opTok, right));
    }

    return left;
}


