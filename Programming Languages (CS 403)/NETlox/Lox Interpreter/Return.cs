﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lox_Interpreter
{
    /// <summary>
    /// Represents the return exception, which is invoked in order to return from a function call.
    /// </summary>
    internal class Return : Exception
    {
        public readonly object? value;

        /// <summary>
        /// Initializes an instance of the <see cref="Return"/> class with the specified value.
        /// </summary>
        /// <param name="value">Value to be returned, can be <see langword="null"/>.</param>
        public Return(object? value) : base(null)
        {
            this.value = value;
        }
    }
}
