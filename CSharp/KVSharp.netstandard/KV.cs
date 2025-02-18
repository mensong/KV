using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace KVSharp
{
    public static class KV
    {
        static string m_nativeDllDir;
        static IntPtr m_hDll = IntPtr.Zero;

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool SetDllDirectory(string lpPathName);

        [DllImport("kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.Winapi)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool IsWow64Process([In] IntPtr process, [Out] out bool wow64Process);

        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary", SetLastError = true)]
        public static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);


        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary", SetLastError = true)]
        public static extern bool FreeLibrary(IntPtr hModule);

        public static void Init()
        {
            //https://stackoverflow.com/questions/4385806/determine-the-loaded-path-for-dlls/4385997
            SetDllDirectory(null); // Reset.
            SetDllDirectory("");   // Plug "binary planting" security hole.

            string dllpath;
            bool wow64Process = true;
            IsWow64Process(Process.GetCurrentProcess().Handle, out wow64Process);
            if (wow64Process)
                dllpath = "runtimes\\win-x86\\native";
            else
                dllpath = "runtimes\\win-x64\\native";

            m_nativeDllDir = $"{AppDomain.CurrentDomain.BaseDirectory}{dllpath}";  //BaseDirectory 有分隔符结尾
            Console.WriteLine(m_nativeDllDir);
            bool ret = SetDllDirectory(m_nativeDllDir);
            if (!ret)
                throw new System.ComponentModel.Win32Exception();

            m_hDll = LoadLibrary("KV.dll");

            SetDllDirectory(null); // Reset.
            SetDllDirectory("");   // Plug "binary planting" security hole.
        }

        [DllImport("KV.dll", EntryPoint = "SetStrA", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetStr(string k, string v);

        [DllImport("KV.dll", EntryPoint = "GetStrA", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetStr(string k, string def);
        public static string GetStr(string k, string def = "")
        {
            IntPtr p = _GetStr(k, def);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "HasStrA", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool HasStr(string k);

        [DllImport("KV.dll", EntryPoint = "DelStrA", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DelStr(string k);


        [DllImport("KV.dll", EntryPoint = "StrAKeysCount", CallingConvention = CallingConvention.Cdecl)]
        public static extern int StrKeysCount();

        [DllImport("KV.dll", EntryPoint = "GetStrAKey", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetStrAKey(int keyIdx);
        public static string GetStrKey(int keyIdx)
        {
            IntPtr p = _GetStrAKey(keyIdx);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "SetInt", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetInt(string k, int v);

        [DllImport("KV.dll", EntryPoint = "GetInt", CallingConvention = CallingConvention.Cdecl)]
        private static extern int _GetInt(string k, int def);
        public static int GetInt(string k, int def = 0)
        {
            return _GetInt(k, def);
        }

        [DllImport("KV.dll", EntryPoint = "HasInt", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool HasInt(string k);

        [DllImport("KV.dll", EntryPoint = "DelInt", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DelInt(string k);

        [DllImport("KV.dll", EntryPoint = "IntKeysCount", CallingConvention = CallingConvention.Cdecl)]
        public static extern int IntKeysCount();

        [DllImport("KV.dll", EntryPoint = "GetIntKey", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetIntKey(int keyIdx);
        public static string GetIntKey(int keyIdx)
        {
            IntPtr p = _GetIntKey(keyIdx);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "SetDouble", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetDouble(string k, double v);

        [DllImport("KV.dll", EntryPoint = "GetDouble", CallingConvention = CallingConvention.Cdecl)]
        private static extern double _GetDouble(string k, double def);
        public static double GetDouble(string k, double def = 0.0)
        {
            return _GetDouble(k, def);
        }

        [DllImport("KV.dll", EntryPoint = "HasDouble", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool HasDouble(string k);

        [DllImport("KV.dll", EntryPoint = "DelDouble", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DelDouble(string k);

        [DllImport("KV.dll", EntryPoint = "DoubleKeysCount", CallingConvention = CallingConvention.Cdecl)]
        public static extern int DoubleKeysCount();

        [DllImport("KV.dll", EntryPoint = "GetDoubleKey", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetDoubleKey(int keyIdx);
        public static string GetDoubleKey(int keyIdx)
        {
            IntPtr p = _GetDoubleKey(keyIdx);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "SetBuff", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetBuff(string k, byte[] buff, int buffLen);

        [DllImport("KV.dll", EntryPoint = "GetBuff", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetBuff(string k, out int outLen);
        public static byte[] GetBuff(string k)
        {
            int outLen = 0;
            IntPtr p = _GetBuff(k, out outLen);
            if (p == IntPtr.Zero)
                return null;

            byte[] bytes = new byte[outLen];
            if (outLen == 0)
                return bytes;
            Marshal.Copy(p, bytes, 0, (int)outLen);
            return bytes;
        }

        [DllImport("KV.dll", EntryPoint = "HasBuff", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool HasBuff(string k);

        [DllImport("KV.dll", EntryPoint = "DelBuff", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DelBuff(string k);

        [DllImport("KV.dll", EntryPoint = "Encrypt", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _Encrypt(string data, string desKey);
        public static string Encrypt(string data, string desKey)
        {
            IntPtr p = _Encrypt(data, desKey);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "Decrypt", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _Decrypt(string data, string desKey);
        public static string Decrypt(string data, string desKey)
        {
            IntPtr p = _Decrypt(data, desKey);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "InitEncryData", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool InitEncryData(string encryFilePath);

        [DllImport("KV.dll", EntryPoint = "GetDecrypt", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetDecrypt(string k, string def);
        public static string GetDecrypt(string k, string def = "")
        {
            IntPtr p = _GetDecrypt(k, def);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "EncryptData", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _EncryptData(string k, string v);
        public static string EncryptData(string k, string v)
        {
            IntPtr p = _EncryptData(k, v);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "EncryptDataKeysCount", CallingConvention = CallingConvention.Cdecl)]
        public static extern int EncryptDataKeysCount();

        [DllImport("KV.dll", EntryPoint = "GetEncryptDataKey", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetEncryptDataKey(int keyIdx);
        public static string GetEncryptDataKey(int keyIdx)
        {
            IntPtr p = _GetEncryptDataKey(keyIdx);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

        [DllImport("KV.dll", EntryPoint = "InitSharedMem", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool InitSharedMem(string globalName, int maxDataCount, int perDataSize);

        [DllImport("KV.dll", EntryPoint = "SetSharedMem", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetSharedMem(int dataID, string v);

        [DllImport("KV.dll", EntryPoint = "GetSharedMem", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr _GetSharedMem(int dataID);
        public static string GetSharedMem(int dataID)
        {
            IntPtr p = _GetSharedMem(dataID);
            if (p == IntPtr.Zero)
                return null;

            //return Marshal.PtrToStringAnsi(p);

            int length = 0;
            while (Marshal.ReadByte(p, length) != 0)
                length++; // 计算字符串长度（不包括结尾的\0字符）
            byte[] byteArray = new byte[length];
            Marshal.Copy(p, byteArray, 0, length);//复制数据到byte数组
            string result = Encoding.Default.GetString(byteArray); //转UTF-8编码
            return result;
        }

    }

}
