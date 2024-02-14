namespace ProjectNothing
{
    public abstract class INetProtocolGenerator
    {
        public abstract INetProtocol Generate ();
    }

    public sealed class NetProtocolGenerator<T> : INetProtocolGenerator where T : INetProtocol, new()
    {
        public override INetProtocol Generate ()
        {
            return new T ();
        }
    }
}
