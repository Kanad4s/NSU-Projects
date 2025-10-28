using System.Text;

namespace DiningPhilosophers.App.Shared;

public static class Names
{
    public static IReadOnlyList<string> ReadNamesOrDefault(string path)
    {
        if (!File.Exists(path))
        {
            return ["Платон", "Аристотель", "Сократ", "Декарт", "Кант"];
        }
        var names = File.ReadAllLines(path, Encoding.UTF8)
            .Select(l => l.Trim())
            .Where(l => !string.IsNullOrWhiteSpace(l))
            .Take(5)
            .ToArray();
        return names.Length > 0
            ? names
            : ["Платон", "Аристотель", "Сократ", "Декарт", "Кант"];
    }
}

