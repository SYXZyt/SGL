namespace SGLNet
{
    public interface IUniformBuffer : IDisposable
    {
        public IntPtr Handle { get; }

        public void Bind(uint slot);
        public void Upload();

        public bool ImGuiEdit();
    }
}