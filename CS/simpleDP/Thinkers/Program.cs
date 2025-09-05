// See https://aka.ms/new-console-template for more information
using System.Reflection;

namespace Thinkers
{
    class Program
    {
        static void Main()
        {   
            Console.WriteLine("Hello, World!");
            var appDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            Console.WriteLine(appDir);
            FilePhilosopherNameProvider a = new FilePhilosopherNameProvider("Infrastructure/Persistence/PhilosopherNames");
            Console.WriteLine(a.GetNames().Count());
            var names = a.GetNames();
            foreach (var item in names)
            {
                Console.WriteLine(item);
            }
        }
    }
}

