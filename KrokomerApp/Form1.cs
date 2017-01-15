using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace KrokomerApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            int a;
            InitializeComponent();

            SerialPort port = connectPort();
            string bb = port.ReadExisting();
        }

        private static SerialPort connectPort(){
            string[] a = SerialPort.GetPortNames();
            SerialPort mySerialPort = new SerialPort(a.First());

            mySerialPort.BaudRate = 9600;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
            mySerialPort.Open();
            
            return mySerialPort;
        }
    }    
}
