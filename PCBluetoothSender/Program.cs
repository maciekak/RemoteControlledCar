using System;
using System.IO.Ports;
using System.Text;

namespace ConsoleApp2
{
    class Program
    {
        static byte GetEncodedChar(char input)
        {
            var digitOffset = '9' - '0' + 1;
            var smallCharOffset = 'z' - 'a' + digitOffset + 1;
            if (input >= '0' && input <= '9')
            {
                return (byte)(input - '0');
            }
            if (input >= 'a' && input <= 'z')
            {
                return (byte)(input - 'a' + digitOffset);
            }
            var remaingChars = new[] { ' ', ':', ';', '?', '!' };
            int currentOffset = smallCharOffset;
            for (int i = 0; i < remaingChars.Length; i++)
            {
                if(input == remaingChars[i])
                {
                    return (byte) currentOffset;
                }

                currentOffset++;
            }

            return 63;
        }

        static byte[] GetEncodedString(string str)
        {
            var res = new byte[str.Length];
            for (int i = 0; i < str.Length; i++)
            {
                byte ch = GetEncodedChar(str[i]);
                res[i] = ch;
            }

            return res;
        }

        static char GetDecodedChar(byte input)
        {
            var digitOffset = '9' - '0' + 1;
            var smallCharOffset = 'z' - 'a' + digitOffset + 1;
            if (input >= 0 && input <= 9)
            {
                return (char)(input + '0');
            }
            if (input >= digitOffset && input <= smallCharOffset - 1)
            {
                return (char)(input + 'a' - digitOffset);
            }
            var remaingChars = new[] { ' ', ':', ';', '?', '!' };
            int currentOffset = smallCharOffset;
            for (int i = 0; i < remaingChars.Length; i++)
            {
                if (input == currentOffset)
                {
                    return remaingChars[i];
                }

                currentOffset++;
            }

            return 'E';
        }

        static string GetDecodedString(byte[] input)
        {
            var res = new StringBuilder();
            for (int i = 0; i < input.Length; i++)
            {
                char ch = GetDecodedChar(input[i]);
                res.Append(ch);
            }

            return res.ToString();
        }

        static byte[] PrepareBytesToSend(byte[] input)
        {
            var result = new byte[input.Length + 1];
            result[0] = 0;
            for(var i = 0; i < input.Length; i++)
            {
                result[i + 1] = input[i];
            }

            return result;
        }

        static void Main(string[] args)
        {
            var names = SerialPort.GetPortNames();
            var port = new SerialPort("COM5", 9600, Parity.None, 8);
            var chars = new byte[33];
            for(byte i = 1; i < 33; i++)
            {
                chars[i] = (byte)(i-1);
            }
            //        {
            //    0b00000001,
            //    0b00000010,
            //    0b00000100,
            //    0b00001000,
            //    0b00010000,
            //    0b00100000,
            //    0b01000000,
            //    0b10000000,
            //    0b10000001,
            //    0b10000010,
            //    0b10000100,
            //    0b10001000,
            //    0b10010000,
            //    0b10100000,
            //    0b11000000,
            //    0b00000000,
            //};
            var a = GetEncodedChar('a');
            var c3 = GetEncodedChar('3');
            var Z = GetEncodedChar(' ');
            var z = GetEncodedChar(':');
            var z1 = GetEncodedChar(';');
            var z2 = GetEncodedChar('!');
            var z3 = GetEncodedChar('?');
            var str = GetEncodedString("adam to cwel!:?;");
            var str2 = GetDecodedString(str);
            var toSend = PrepareBytesToSend(str);
            var chars2 = new byte[7];
            chars2[0] = 0;
            chars2[1] = 65;
            chars2[2] = 80;
            chars2[3] = 96;
            chars2[4] = 128;
            chars2[5] = 127;
            chars2[6] = 1;
            port.Open();
            //port.Write(chars2, 0, 3);
            //port.Write(chars2, 0, 4);
            //port.Write(chars2, 0, 5);
            //port.Write(chars2, 0, 6);
            //port.Write(chars2, 0, 7);
            port.Write(toSend, 0, toSend.Length);
            //port.Write(chars, 128, 128);
            Console.WriteLine("Hello World!");
        }
    }
}
