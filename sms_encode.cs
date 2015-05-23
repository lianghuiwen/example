/*
1、短信中心地址长度：包括‘91’和‘F’（单位：字节）
2、短信中心地址类型：国际格式‘91’
3、短信中心地址SCA：前面要加‘86’，补‘F’凑成偶数
4、信息头字节：发：‘11’正常发送、‘31’需要状态报告、‘51’有报头短信；收：TP-RP TP-UDHI TP-SRR TP-VPF[2] TP-RD TP-MTI[2]
5、消息基准值TP-MR：发：‘00’；收：无
6、目标号码长度：不包括‘91’和‘F’（单位：位数）
7、目标号码格式：国际格式‘91’
8、目标号码TPA：前面要加‘86’，补‘F’凑成偶数
9、协议标识TP-PID：‘00’
10、用户信息编码方式TP-DCS：‘00’7-bit、‘08’UCS2、‘15’8-bit（‘F5’UTF-8）(bit4=1闪信)
11、有效期TP-VP：发：‘00’-‘8F’，(VP+1)*5分钟；收：时间戳TP-SCTS，14位
12、信息长度TP-UDL：7-bit编码：指原始短消息的字符个数；8-bit、UCS2编码：指编码后字节数；有信息头：等于头长度与编码后字节数之和。
13、[0]信息头总长度：各个IE长度总和
14、[2]Information Element Identifier：‘05’表示WAP PUSH
15、[2]Information Element Length：‘04’
16、[2]Destination Port：‘0B84’
17、[2]Originating Port：‘23F0’
18、[1]Information Element Identifier：‘00’表示长短信，8-bit reference number；‘08’表示长短信，16-bit reference number；（对于7-bit编码超长短信，协议头要凑够7位长度）
19、[1]Information Element Length：‘03’
20、[1]Reference Number　 
21、[1]Total Packet　 
22、[1]Current Packet　 
23、[2]WSP Header：‘29060603AE81EA8DCA’
24、[2]XML固定格式：‘02056A0045C6080C03’
25、[2]URL：不带http://
26、[2]XML固定格式：‘000103’
27、内容 
28、[2]XML固定格式：‘000101’
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
namespace Sms
{
    enum PduEncoding { Bit7 = 1, UCS2 = 2, UTF8 = 3 }
    class SmParam
    {
        public byte totalPacket = 1;
        public byte currentPacket = 1;
        public string fromNumber = "";
        public string sendTime = "";
        public string url = "";
        public string msgContent = "";
        public PduEncoding encoding = PduEncoding.Bit7;
        override public string ToString()
        {
            string str = String.Format("[{0}]{1}:", sendTime, fromNumber);
            if (totalPacket > 1)
            {
                str = String.Format("{0}{1}/{2}", str, currentPacket, totalPacket);
            }
            if (url.Length > 0)
            {
                str = String.Format("{0}<{1}>", str, url);
            }
            str = String.Format("{0}{1}", str, msgContent);
            return str;
        }
    }
    class SmsPdu
    {
        private string InvertNumbers(string num)
        {
            string str = "";
            if (num.Length > 0)
            {
                num = "86" + num;
                if (num.Length % 2 != 0)
                {
                    num = num + "F";
                }
                for (int i = 0; i < num.Length; i += 2)
                {
                    str += num[i + 1];
                    str += num[i];
                }
                str = "91" + str;
            }
            return str;
        }
        private string SerializeNumbers(string num)
        {
            string str = "";
            if (num.Length > 0)
            {
                for (int i = 0; i < num.Length; i += 2)
                {
                    str += num[i + 1];
                    str += num[i];
                }
                if (str.StartsWith("86"))
                {
                    str = str.Remove(0, 2);
                }
                if (str.EndsWith("F"))
                {
                    str = str.Remove(str.Length - 1);
                }
            }
            return str;
        }
        private string SerializeSendTime(string time)
        {
            string str = "";
            for (int i = 0; i < time.Length; i += 2)
            {
                str += time[i + 1];
                str += time[i];
            }
            string year = str.Substring(0, 2);
            string mon = str.Substring(2, 2);
            string date = str.Substring(4, 2);
            string hour = str.Substring(6, 2);
            string min = str.Substring(8, 2);
            string sec = str.Substring(10, 2);
            return String.Format("{0}-{1}-{2} {3}:{4}:{5}", year, mon, date, hour, min, sec);
        }
        public string EncodeUCS2(string message)
        {
            string str = "";
            //byte[] buf = Encoding.BigEndianUnicode.GetBytes(message);
            //for (int i = 0; i < buf.Length; i++)
            //{
            //    str += buf[i].ToString("X2");
            //}
            byte[] buf = Encoding.Unicode.GetBytes(message);
            for (int i = 0; i < buf.Length; i += 2)
            {
                str += buf[i + 1].ToString("X2");
                str += buf[i].ToString("X2");
            }
            return str;
        }
        public string DecodeUCS2(string message)
        {
            byte[] buf = new byte[message.Length / 2];
            for (int i = 0; i <= message.Length - 4; i += 4)
            {
                buf[i / 2] = byte.Parse(message.Substring(i + 2, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                buf[i / 2 + 1] = byte.Parse(message.Substring(i, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
            }
            return Encoding.Unicode.GetString(buf);
        }
        public string EncodeUTF8(string message)
        {
            string str = "";
            byte[] buf = Encoding.UTF8.GetBytes(message);
            for (int i = 0; i < buf.Length; i++)
            {
                str += buf[i].ToString("X2");
            }
            return str;
        }
        public string DecodeUTF8(string message)
        {
            byte[] buf = new byte[message.Length / 2];
            for (int i = 0; i <= message.Length - 2; i += 2)
            {
                buf[i / 2] = byte.Parse(message.Substring(i, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
            }
            return Encoding.UTF8.GetString(buf);
        }
        public string EncodeBit7(string message)
        {
            int leftPart = 0;
            string bit7Str = "";
            for (int i = 0; i < message.Length; i++)
            {
                int n = i & 7;
                int curChar = message[i];
                if (n == 0)
                {
                    leftPart = curChar;
                }
                else
                {
                    bit7Str = String.Format("{0}{1:X2}", bit7Str, (byte)(curChar << (8 - n) | leftPart));
                    leftPart = curChar >> n;
                }
            }
            if (leftPart != 0)
            {
                bit7Str = String.Format("{0}{1:X2}", bit7Str, (byte)leftPart);
            }
            return bit7Str;
        }
        public string DecodeBit7(string message)
        {
            int n = 0;
            int leftPart = 0;
            string str = "";
            for (int i = 0; i < message.Length - 2; i += 2)
            {
                byte tmp = byte.Parse(message.Substring(i, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                str += (((tmp << n) | leftPart) & 0x7F).ToString("X2");
                leftPart = tmp >> (7 - n);
                n++;
                if (n == 7)
                {
                    str += leftPart.ToString("X2");
                    n = 0;
                    leftPart = 0;
                }
            }
            byte[] buf = new byte[str.Length / 2];
            for (int i = 0; i <= str.Length - 2; i += 2)
            {
                buf[i / 2] = byte.Parse(str.Substring(i, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
            }
            return Encoding.ASCII.GetString(buf);
        }
        private string LongSmsHeader(byte totalPacket, byte currentPacket)
        {
            byte IEI = 0x00;
            byte IELength = 0x03;
            byte referenceNumber = 0xA0;
            return String.Format("{0:X2}{1:X2}{2:X2}{3:X2}{4:X2}", IEI, IELength, referenceNumber, totalPacket, currentPacket);
        }
        private string LongSmsHeaderBit7(byte totalPacket, byte currentPacket)
        {
            byte IEI = 0x08;
            byte IELength = 0x04;
            ushort referenceNumber = 0x00A0;
            return String.Format("{0:X2}{1:X2}{2:X4}{3:X2}{4:X2}", IEI, IELength, referenceNumber, totalPacket, currentPacket);
        }
        private string PushSmsHeader()
        {
            byte IEI = 0x05;
            byte IELength = 0x04;
            ushort destinationPort = 0x0B84;
            ushort originatingPort = 0x23F0;
            return String.Format("{0:X2}{1:X2}{2:X4}{3:X4}", IEI, IELength, destinationPort, originatingPort);
        }
        private string PushSmsBody(string url, string msg)
        {
            string wspHeader = "29060603AE81EA8DCA";
            string afterWsp = "02056A0045C6080C03";
            string afterUrl = "000103";
            string afterMsg = "000101";
            return wspHeader + afterWsp + url + afterUrl + msg + afterMsg;
        }
        private string SCA(string smsc)
        {
            string sca = InvertNumbers(smsc);
            byte scaLength = (byte)(sca.Length / 2);
            return String.Format("{0:X2}{1}", scaLength, sca);
        }
        private string TPA(string number)
        {
            string tpa = InvertNumbers(number);
            byte tpaLength = (byte)(number.Length + 2);
            return String.Format("{0:X2}{1}", tpaLength, tpa);
        }
        public string EncodePdu(string number, string message, PduEncoding encoding)
        {
            return EncodePdu(number, message, "", 1, 1, encoding);
        }
        public string EncodePdu(string number, string message, string url)
        {
            return EncodePdu(number, message, url, 1, 1, PduEncoding.UTF8);
        }
        public string EncodePdu(string number, string message, byte totalPacket, byte currentPacket, PduEncoding encoding)
        {
            return EncodePdu(number, message, "", totalPacket, currentPacket, encoding);
        }
        public string EncodePdu(string number, string message, string url, byte totalPacket, byte currentPacket, PduEncoding encoding)
        {
            bool isLongSms = totalPacket > 1 ? true : false;
            bool isPushSms = url.Length > 0 ? true : false;
            string smsc = "";
            byte tp_head = 0x11;
            byte tp_mr = 0x00;
            byte tp_pid = 0x00;
            byte tp_dcs = 0x00;
            byte tp_vp = 0x00;
            byte tp_udl = 0x00;
            byte tp_udhl = 0x00;
            string tp_udh = "";
            string tp_ud = "";
            if (isPushSms)
            {
                tp_head = 0x51;
                tp_dcs = 0xF5;
                tp_ud = PushSmsBody(EncodeUTF8(url), EncodeUTF8(message));
                tp_udh = PushSmsHeader();
                if (isLongSms)
                {
                    tp_udh += LongSmsHeader(totalPacket, currentPacket);
                }
                tp_udhl = (byte)(tp_udh.Length / 2);
                tp_udl = (byte)((tp_udh.Length + tp_ud.Length) / 2);
            }
            else
            {
                if (encoding == PduEncoding.Bit7)
                {
                    tp_dcs = 0x00;
                    tp_ud = EncodeBit7(message);
                    if (isLongSms)
                    {
                        tp_head = 0x51;
                        tp_udh = LongSmsHeaderBit7(totalPacket, currentPacket);
                        tp_udhl = (byte)(tp_udh.Length / 2);
                        tp_udl = (byte)((tp_udh.Length + tp_ud.Length) / 20);
                    }
                    else
                    {
                        tp_udl = (byte)(message.Length);
                    }
                }
                else if (encoding == PduEncoding.UCS2)
                {
                    tp_dcs = 0x08;
                    tp_ud = EncodeUCS2(message);
                    if (isLongSms)
                    {
                        tp_head = 0x51;
                        tp_udh = LongSmsHeader(totalPacket, currentPacket);
                        tp_udhl = (byte)(tp_udh.Length / 2);
                    }
                    tp_udl = (byte)((tp_udh.Length + tp_ud.Length) / 2);
                }
            }
            string pdu = "";
            pdu += SCA(smsc);
            pdu += tp_head.ToString("X2");
            pdu += tp_mr.ToString("X2");
            pdu += TPA(number);
            pdu += tp_pid.ToString("X2");
            pdu += tp_dcs.ToString("X2");
            pdu += tp_vp.ToString("X2");
            pdu += tp_udl.ToString("X2");
            if (isLongSms || isPushSms)
            {
                pdu += tp_udhl.ToString("X2");
                pdu += tp_udh;
            }
            pdu += tp_ud;
            return pdu;
        }
        public SmParam DecodePdu(string pduString)
        {
            string debugmsg = "";
            SmParam smParam = new SmParam();
            try
            {
                int pos = 0;
                byte scaLength = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                debugmsg = String.Format("{0}scaLength: {1}/n", debugmsg, pduString.Substring(pos, 2));
                scaLength *= 2;
                pos += 2;
                if (scaLength > 0)
                {
                    string sca = pduString.Substring(pos, scaLength);
                    debugmsg = String.Format("{0}sca: {1}/n", debugmsg, pduString.Substring(pos, scaLength));
                    pos += scaLength;
                }
                //string headerByte = pduString.Substring(pos, 2);
                byte headerByte = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.HexNumber);
                debugmsg = String.Format("{0}headerByte: {1}/n", debugmsg, pduString.Substring(pos, 2));
                pos += 2;
                byte tpaLength = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                debugmsg = String.Format("{0}tpaLength: {1}/n", debugmsg, pduString.Substring(pos, 2));
                if (tpaLength % 2 != 0)
                {
                    tpaLength += 1;
                }
                pos += 2;
                pos += 2;
                string tpa = pduString.Substring(pos, tpaLength);
                debugmsg = String.Format("{0}tpa: {1}/n", debugmsg, pduString.Substring(pos, tpaLength));
                pos += tpaLength;
                smParam.fromNumber = SerializeNumbers(tpa);
                string tp_pid = pduString.Substring(pos, 2);
                debugmsg = String.Format("{0}tp_pid: {1}/n", debugmsg, pduString.Substring(pos, 2));
                pos += 2;
                string tp_dcs = pduString.Substring(pos, 2);
                debugmsg = String.Format("{0}tp_dcs: {1}/n", debugmsg, pduString.Substring(pos, 2));
                pos += 2;
                string tp_scts = pduString.Substring(pos, 14);
                debugmsg = String.Format("{0}tp_scts: {1}/n", debugmsg, pduString.Substring(pos, 14));
                pos += 14;
                smParam.sendTime = SerializeSendTime(tp_scts);
                byte tp_udl = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                debugmsg = String.Format("{0}tp_udl: {1}/n", debugmsg, pduString.Substring(pos, 2));
                pos += 2;
                ushort tp_udhl = 0;
                bool isLongSms = false;
                bool isPushSms = false;
                //if (headerByte == "44" || headerByte == "40" || headerByte == "60" || headerByte == "64")
                if ((headerByte & 0x40) == 0x40)
                {
                    tp_udhl = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                    debugmsg = String.Format("{0}tp_udhl: {1}/n", debugmsg, pduString.Substring(pos, 2));
                    pos += 2;
                    int tempPos = 0;
                    while (true)
                    {
                        string iei = pduString.Substring(pos, 2);
                        debugmsg = String.Format("{0}iei: {1}/n", debugmsg, pduString.Substring(pos, 2));
                        pos += 2;
                        byte ieiLength = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                        debugmsg = String.Format("{0}ieiLength: {1}/n", debugmsg, pduString.Substring(pos, 2));
                        pos += 2;
                        switch (iei)
                        {
                            case "00":
                                isLongSms = true;
                                pos += 2;
                                smParam.totalPacket = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                                debugmsg = String.Format("{0}totalPacket: {1}/n", debugmsg, pduString.Substring(pos, 2));
                                pos += 2;
                                smParam.currentPacket = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                                debugmsg = String.Format("{0}currentPacket: {1}/n", debugmsg, pduString.Substring(pos, 2));
                                pos += 2;
                                break;
                            case "08":
                                isLongSms = true;
                                pos += 4;
                                smParam.totalPacket = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                                debugmsg = String.Format("{0}totalPacket: {1}/n", debugmsg, pduString.Substring(pos, 2));
                                pos += 2;
                                smParam.currentPacket = byte.Parse(pduString.Substring(pos, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                                debugmsg = String.Format("{0}currentPacket: {1}/n", debugmsg, pduString.Substring(pos, 2));
                                pos += 2;
                                break;
                            case "05":
                                isPushSms = true;
                                pos += 4;
                                pos += 4;
                                break;
                        }
                        tempPos += (2 + ieiLength);
                        if (tempPos == tp_udhl) break;
                    }
                }
                string url = "";
                string msg = pduString.Substring(pos);
                if (isPushSms)
                {
                    string mmsMessage = "6170706C69636174696F6E2F766E642E7761702E6D6D732D6D657373616765";
                    if (msg.IndexOf(mmsMessage) >= 0)
                    {
                        string beforeUrl = "008D9083";
                        string afterUrl = "008805810302A300891780";
                        string afterMsg = "008A808E";
                        int urlStartPos = msg.IndexOf(beforeUrl) >= 0 ? msg.IndexOf(beforeUrl) + beforeUrl.Length : -1;
                        int urlEndPos = msg.IndexOf(afterUrl);
                        int msgStartPos = msg.IndexOf(afterUrl) >= 0 ? msg.IndexOf(afterUrl) + afterUrl.Length : -1;
                        int msgEndPos = msg.IndexOf(afterMsg);
                        debugmsg = String.Format("{0}mmsMessage: {1}/n", debugmsg, mmsMessage);
                        debugmsg = String.Format("{0}beforeUrl: {1}/n", debugmsg, beforeUrl);
                        if (urlStartPos >= 0 && urlEndPos >= 0 && urlEndPos > urlStartPos)
                        {
                            url = msg.Substring(urlStartPos, urlEndPos - urlStartPos);
                            debugmsg = String.Format("{0}url: {1}/n", debugmsg, url);
                        }
                        debugmsg = String.Format("{0}afterUrl: {1}/n", debugmsg, afterUrl);
                        if (msgStartPos >= 0 && msgEndPos >= 0 && msgEndPos > msgStartPos)
                        {
                            msg = msg.Substring(msgStartPos, msgEndPos - msgStartPos);
                            debugmsg = String.Format("{0}msg: {1}/n", debugmsg, msg);
                        }
                        debugmsg = String.Format("{0}afterMsg: {1}/n", debugmsg, afterMsg);
                    }
                }
                switch (tp_dcs)
                {
                    case "00":
                        smParam.encoding = PduEncoding.Bit7;
                        smParam.msgContent = DecodeBit7(msg);
                        break;
                    case "08":
                        smParam.encoding = PduEncoding.UCS2;
                        smParam.msgContent = DecodeUCS2(msg);
                        break;
                    default:
                        smParam.encoding = PduEncoding.UTF8;
                        smParam.msgContent = DecodeUTF8(msg);
                        smParam.url = DecodeUTF8(url);
                        break;
                }
                debugmsg = String.Format("{0}msg: {1}/n", debugmsg, msg);
                debugmsg = String.Format("{0}msg: {1}/n", debugmsg, smParam.msgContent);
                StreamWriter sw = new StreamWriter("decode.txt", true, Encoding.Unicode);
                sw.WriteLine(pduString);
                sw.WriteLine(debugmsg);
                sw.Close();
                return smParam;
            }
            catch (System.Exception e)
            {
                return smParam;
            }
        }
    }
}