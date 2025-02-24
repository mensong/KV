using KVSharp;

namespace TestNet
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            KV.Init();
            KV.InitSharedMem("MyKV", 10, 512);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            KV.SetSharedMem(0, "MENSONG");

        }

        private void button2_Click(object sender, EventArgs e)
        {
            MessageBox.Show(KV.GetSharedMem(0));
        }
    }
}
