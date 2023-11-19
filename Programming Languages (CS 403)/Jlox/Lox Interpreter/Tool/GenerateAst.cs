using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace Lox_Interpreter.Tool
{
    /// <summary>
    /// Represents the generation expression classes for Lox syntax trees.
    /// </summary>
    public class GenerateAst
    {
        /// <summary>
        /// Creates a new class that represents the grammar of Lox. Comment out when running main Lox class.
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
                "Binary   : Expr left, Token oper, Expr right",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Unary    : Token oper, Expr right"};

            DefineAst(outputDir, "Expr", array.ToList());
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

            writer.WriteLine("namespace Lox_Interpreter.Lox");
            writer.WriteLine("{");

            writer.WriteLine("\tpublic abstract class " + baseName);
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
                writer.WriteLine("\t\t\tR Visit" + typeName + baseName + "(" +typeName + " " + baseName.ToLower() + ");");
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
