namespace ProjectNothing.Network
{
    abstract class INetProtocolGenerator
    {
        public abstract INetProtocol Generate ();
    }

    sealed class NetProtocolGenerator<T> : INetProtocolGenerator where T : NetProtocol<T>, new ()
    {
        public override INetProtocol Generate ()
        {
            return new T ();
        }
    }
}
