namespace SGLNet
{
    public static class Extensions
    {
        public static bool TryAdd<T>(this List<T> list, T item)
        {
            if (!list.Contains(item))
            {
                list.Add(item);
                return true;
            }

            return false;
        }
    }
}