namespace ProjectNothing.Network
{
    public abstract class INetProtocolGenerator
    {
        public abstract INetProtocol Generate ();
    }

    public sealed class NetProtocolGenerator<T> : INetProtocolGenerator where T : NetProtocol<T>, new()
    {
        public override INetProtocol Generate ()
        {
            return new T ();
        }
    }
}
