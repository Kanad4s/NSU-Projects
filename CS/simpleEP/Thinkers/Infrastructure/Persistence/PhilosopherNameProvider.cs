public class FilePhilosopherNameProvider : IPhilosopherNameProvider
{
    private readonly string _filePath;
    private IEnumerable<string> _names;

    public FilePhilosopherNameProvider(string filePath)
    {
        _filePath = filePath;
        _names = File.ReadAllLines(_filePath).Where(line => !string.IsNullOrWhiteSpace(line));
    }

    public IEnumerable<string> GetNames()
    {
        return File.ReadAllLines(_filePath).Where(line => !string.IsNullOrWhiteSpace(line));
    }

    public string GetName()
    {
        var name = _names.Last();
        _names.Last();
        return "";
    }
}
