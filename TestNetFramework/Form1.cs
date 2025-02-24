using KVSharp;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestNetFramework
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
            KV.SetSharedMem("MyKV", 0, "MENSONG");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MessageBox.Show(KV.GetSharedMem("MyKV", 0));
        }
    }
}
