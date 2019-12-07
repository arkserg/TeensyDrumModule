using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary
{
    public static class TypeExtensions
    {
        public static IEnumerable<Type> GetInheritedClasses(this Type type)
        {
            return Assembly.GetAssembly(type).GetTypes()
                .Where(myType => myType.IsClass && !myType.IsAbstract && myType.IsSubclassOf(type));
        }
    }
}