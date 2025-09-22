using CommunityToolkit.Mvvm.Messaging.Messages;

namespace ConfigParser
{
    public class MessageData : ValueChangedMessage<string>
    {
        public MessageData(string value) : base(value)
        {

        }
    }
}
