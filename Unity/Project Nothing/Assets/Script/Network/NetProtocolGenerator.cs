namespace ProjectNothing.Network
{
    public abstract class INetProtocolGenerator
    {
        public abstract INetProtocol Generate ();
    }

    public sealed class NetCommandGenerator<T> : INetProtocolGenerator where T : NetCommand<T>, new()
    {
        public override INetProtocol Generate ()
        {
            return new T ();
        }
    }

    public sealed class NetEventGenerator<T> : INetProtocolGenerator where T : NetEvent<T>, new()
    {
        public override INetProtocol Generate ()
        {
            return new T ();
        }
    }
}
