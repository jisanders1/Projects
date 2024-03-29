﻿using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

// Directory of Lox Files is ../../../../"Lox Interpreter"
namespace Ast_Generator
{
    /// <summary>
    /// Represents the generation of expression and statement classes for Lox syntax trees.
    /// </summary>
    internal class GenerateAst
    {
        /// <summary>
        /// Creates 2 new classes that represents the grammar of Lox. One for expressions and another for statements.
        /// </summary>
        /// <param name="args">Path oc new class.</param>
        public static void Main(String[] args)
        {
            if (args.Length != 1)
            {
                Console.Error.WriteLine("Usage: generate_ast <output directory>");
                Environment.Exit(64);
            }
            String outputDir = args[0];
            // Used to implement subclass information
            String[] array = new string[] {
                "Assign   : Token name, Expr value",
                "Binary   : Expr left, Token oper, Expr right",
                "Get      : Expr obj, Token name",
                "Call     : Expr callee, Token paren, List<Expr> arguments",
                "Grouping : Expr expression",
                "Literal  : Object? value",
                "Logical  : Expr left, Token oper, Expr right",
                "Set      : Expr obj, Token name, Expr value",
                "Super    : Token keyword, Token method",
                "This     : Token keyword",
                "Unary    : Token oper, Expr right",
                "Variable : Token name"};

            DefineAst(outputDir, "Expr", array.ToList());

            array = new string[] {
                "Block      : List<Stmt?> statements",
                "Class      : Token name, Expr.Variable? superclass, List<Function> methods",
                "Expression : Expr expression",
                "Function   : Token name, List<Token> parameters, List<Stmt?> body",
                "If         : Expr condition, Stmt thenBranch, Stmt? elseBranch",
                "Print      : Expr expression",
                "Return     : Token keyword, Expr? value",
                "Var        : Token name, Expr? initializer",
                "While      : Expr condition, Stmt body"};
            DefineAst(outputDir, "Stmt", array.ToList());
        }
        /// <summary>
        /// Generates the script that represents an automatic syntax tree for the grammar of Lox.
        /// </summary>
        /// <param name="outputDir">Path of class relative to the current location.</param>
        /// <param name="baseName">Name of class/filename</param>
        /// <param name="types">List containing information about the types of </param>
        private static void DefineAst(String outputDir, String baseName, List<String> types)
        {
            String path = outputDir + "/" + baseName + ".cs";
            using StreamWriter writer = new(path);

            writer.WriteLine("namespace Lox_Interpreter");
            writer.WriteLine("{");

            writer.WriteLine("\tinternal abstract class " + baseName);
            writer.WriteLine("\t{");
            DefineVisitor(writer, baseName, types);
            foreach (String type in types)
            {
                String className = type.Split(':')[0].Trim();
                String fields = type.Split(':')[1].Trim();
                DefineType(writer, baseName, className, fields);
            }
            writer.WriteLine();
            writer.WriteLine("\t\tpublic abstract R Accept<R>(IVisitor<R> visitor);");
            writer.WriteLine("\t}");

            writer.WriteLine("}");
        }

        /// <summary>
        /// Generates interface for the Visitor pattern.
        /// </summary>
        /// <param name="writer">Writes text to the new class file.</param>
        /// <param name="baseName">Name that becomes part of interface methods.</param>
        /// <param name="types">List containing names of different expression types.</param>
        private static void DefineVisitor(StreamWriter writer, String baseName, List<String> types)
        {
            writer.WriteLine("\t\tpublic interface IVisitor<R>");
            writer.WriteLine("\t\t{");

            foreach (String type in types)
            {
                String typeName = type.Split(':')[0].Trim();
                writer.WriteLine("\t\t\tR Visit" + typeName + baseName + "(" + typeName + " " + baseName.ToLower() + ");");
            }
            writer.WriteLine("\t\t}");
        }

        /// <summary>
        /// Generates subclasses of the syntax tree while implementing interface methods.
        /// </summary>
        /// <param name="writer">Writes text to the new class file.</param>
        /// <param name="baseName">Name that becomes the superclass for inheritance.</param>
        /// <param name="className">Name of the subclass.</param>
        /// <param name="fieldList">List of the fields for each subclass.</param>
        private static void DefineType(StreamWriter writer, String baseName, String className, String fieldList)
        {
            writer.WriteLine("\t\tpublic class " + className + " : " + baseName);
            writer.WriteLine("\t\t{");

            // Constructor.
            writer.WriteLine("\t\t\tpublic " + className + "(" + fieldList + ")");
            writer.WriteLine("\t\t\t{");

            // Store parameters in fields.
            String[] seperator = new string[] { ", " };
            String[] fields = fieldList.Split(seperator, StringSplitOptions.RemoveEmptyEntries);
            foreach (String field in fields)
            {
                String name = field.Split(' ')[1];
                writer.WriteLine("\t\t\t\tthis." + name + " = " + name + ";");
            }

            writer.WriteLine("\t\t\t}");

            // Override Accept Method for each Subclass
            writer.WriteLine();
            writer.WriteLine("\t\t\tpublic override R Accept<R>(IVisitor<R> visitor)");
            writer.WriteLine("\t\t\t{");
            writer.WriteLine("\t\t\t\treturn visitor.Visit" + className + baseName + "(this);");
            writer.WriteLine("\t\t\t}");

            // Fields.
            writer.WriteLine();
            foreach (String field in fields)
            {
                writer.WriteLine("\t\t\tpublic readonly " + field + ";");
            }

            writer.WriteLine("\t\t}");
        }
    }
}