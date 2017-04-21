/*
    This file is part of solidity.

    solidity is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    solidity is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author julius <djudju@protonmail.com>
 * @date 2017
 * Converts an AST from json format to an ASTNode
 */
#include <vector>
#include <libsolidity/ast/AST.h>
#include <json/json.h>
#include <libsolidity/ast/ASTAnnotations.h>
#include <libevmasm/SourceLocation.h>

using namespace std;

namespace dev
{
namespace solidity
{
/**
 * takes an AST in Json Format and recreates it with AST-Nodes
 */

ASTNode root;

ASTNode convertJsonToASTNode(Json::Value const& _ast){
    //pseudocode
    switch (_ast["nodeType"])
    {
        case "SourceUnit":
            return createSourceUnit(_ast);
        case "PragmaDirective":
            return createPragmaDirective(_ast);
        case "ImportDirective":
            return createImportDirective(_ast);
        case "ContractDefinition":
            return createContractDefinition(_ast);
        case "InheritanceSpecifier":
            return createInheritanceSpecifier(_ast);
        case "UsingForDirective":
            return createStructDefinition(_ast);
        //add more cases here...
        default:
            someError(string("type of JsonValue is unknown."))
    }
}

//ASTPointer<ASTNode> createX(Json::Value _node){
    //create args for the constructor of the node && see how they are filled in parser.cpp
    //copy from constructor in ast.h
    //create node
    //ASTPointer<ASTNode> tmp = make_shared<X>();
    //tmp->setId(_node["id"])
    //fill the annotation
    //tmp->
    //return tmp;
    //}

ASTPointer<ASTNode> createSourceUnit(Json::Value const& _node){
    //create args for constructor
    vector<ASTPointer<ASTNode>> nodes;
    for (auto& child : _node["nodes"])
        nodes.push_back(convertJsonToAST(child));
    //create node
    ASTPointer<ASTNode> tmp = make_shared<SourceUnit>(, nodes);
    tmp->setId(_node["id"]); //TODO write setId() for ASTNode
    //fill the annotation
    map<ASTString, vector<Declaration const*>> exportedSymbols;
    for (auto& tuple : _node["exportedSymbols"])
        exportedSymbols.insert({tuple.first, tuple.second});
    tmp->annotation.exportedSymbols = exportedSymbols;
    tmp->annotation().path = _node["absolutePath"];
    return tmp;
}

SourceLocation getSourceLocation(Json::Value _node){
        string srcString = _node["src"];
        auto firstColon = std::find(srcString, ":");
        auto secondColon = std::find(srcString, ":", firstColon+1);
        return SourceLocation(
            srcString.substring(0,firstColon),
            srcString.substring(firstColon, secondColon),
            srcString.substring(secondColon)
        );
}

ASTPointer<ASTNode> createPragmaDirective(Json::Value const& _node) //help
{
//      vector<Token::Value> tokens;//here
        vector<ASTString> literals;
        for (auto const& lit : _node["literals"])
            literals.push_back(lit);
        SourceLocation const& location = getSourceLocation(_node);
        ASTPointer<ASTNode> tmp = make_shared<PragmaDirective>(tokens, literals);

}

ASTPointer<ASTNode> createImportDirective(Json::Value _node){
    //create args for the fields of the node
    ASTPointer<ASTString> path = make_shared<ASTString>(_node["file"]);
    ASTPointer<ASTString> unitAlias = make_shared<ASTString>(_node["unitAlias"]);
    std::vector<std::pair<ASTPointer<Identifier>, ASTPointer<ASTString>>> symbolAliases;
    for (auto& tuple : _node["symbolAliases"])
    {
        symbolAliases.push_back(make_pair(
                make_shared<Identifier>(tuple.first),
                make_shared<ASTString>(tuple.second)
        ));
    }
    //create node
    ASTNode tmp = make_shared<ImportDirective>(path, unitAlias,symbolAliases);
    //fill the annotation
    tmp.annotation.absolutePath = make_shared<ASTString>(_node["absolutePath"]);
    //tmp.annotation.sourceUnit = _node(&createSourceUnit(????)) -> how to deal with pointers from the annotation?
    //tmp.annotation.scope = make_shared<Identifier> ??
    return tmp;
    //}
}

ASTNode createContractDefinition(Json::Value _node){
    //create args for the constructor of the node
    SourceLocation const& location = getSourceLocation(_node);
    ASTPointer<ASTString> const& _documentation = make_shared<ASTString>(""); //postponed
    ASTPointer<ASTString> name = make_shared<ASTString>(_node["name"]);
    std::vector<ASTPointer<InheritanceSpecifier>> const& baseContracts;
    for (auto& base : _node["baseContracts"])
        baseContracts.append(createInheritanceSpecifier(base));
    std::vector<ASTPointer<ASTNode>> const& _subNodes;
    for (auto& subnode : _node["nodes"])
        subNodes.append(convertJsonToASTNode(subnode));
    bool _isLibrary = _node["isLibrary"];
    //create node
    ASTPointer<ASTNode> tmp = make_shared<ContractDefinition>(
                name,
                docString,
                baseContracts,
                subNodes,
                _isLibrary
     );
    //fill the annotation
    tmp->annotation.isFullyImplemented = _node["isFullyImplemented"];
    //tmp->annotation
    return *tmp
    }

ASTPointer<ASTNode> createInheritanceSpecifier(Json::Value _node){
    //create args for the constructor of the node && see how they are filled in parser.cpp
    SourceLocation const& location = getSourceLocation(_node);
    ASTPointer<UserDefinedTypeName> const& baseName = createUserDefinedTypeName(_node["baseName"]);
    std::vector<ASTPointer<Expression>> arguments;
    for (auto& arg : _node["arguments"])
        arguments.push_back(createExpression(arg));
    //    create node
    ASTPointer<ASTNode> tmp = make_shared<InheritanceSpecifier>(
        location,
        basename,
        arguments
    );
    //fill the annotation
    return tmp;
    }

ASTPointer<ASTNode> createUsingForDirective(Json::Value _node){
    //create args for the constructor of the node && see how they are filled in parser.cpp
    //copy from constructor in ast.h
    //create node
    //ASTPointer<ASTNode> tmp = make_shared<X>();
    //tmp->setId(_node["id"])
    //fill the annotation
    //tmp->
    //return tmp;
    //}


}
