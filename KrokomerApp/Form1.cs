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
        string kroky;

        System.Windows.Forms.Timer tmr = null;
        TimeSpan _elapsed = new TimeSpan();
        SerialPort port = connectPort();
        public Form1()
        {
            InitializeComponent();
            StartTimer();
            
        }
        void tmr_Tick(object sender, EventArgs e)
        {
            
            _elapsed = _elapsed.Add(TimeSpan.FromMinutes(1));
            textBox1.Text = _elapsed.ToString();

            //citanie z portu a uprava
            string bb = port.ReadExisting();
            bb = bb.Replace("\r", ";");
            string[] array = bb.Split(';');
            string value = array[array.Length - 2];

            if (Convert.ToInt32(value) >= Convert.ToInt32(kroky))
            {
                // vykonal sa krok
                getValue(value);
                kroky = value;

            }
            else
            {
                Application.Restart();
            }

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

        private void getValue(string stepCount)
        {
            double distance = (Convert.ToInt32(stepCount) * Convert.ToDouble(textBox3.Text));
            double speed = (distance/Convert.ToDouble(_elapsed.Minutes))*3.6;
            textBox2.Text = stepCount;                    //number of steps output
            textBox2.ForeColor = Color.Red;
            textBox4.Text = speed.ToString() + " km/h";             //avg. speed
            textBox5.Text = distance.ToString() + " m";   //distance outut

        }

        private void StartTimer()
        {
            tmr = new System.Windows.Forms.Timer();
            tmr.Interval = 1000;
            tmr.Tick += new EventHandler(tmr_Tick);
            tmr.Enabled = true;
        }
    }
    /*private void parse(string input){
            /*
            double value;
            string temp = input.Replace(";", "");
            string[] subStrings = temp.Split(':');

            kroky = int.Parse(subStrings[1]);

            value = double.Parse(subStrings[3]);
            hodnota1 = addElement(hodnota1, value);

            value = double.Parse(subStrings[5]);
            trashhold = addElement(trashhold, value);

            value = double.Parse(subStrings[7]);
            min = addElement(min, value);

            value = double.Parse(subStrings[9]);
            max = addElement(max, value);

            reset = Boolean.Parse(subStrings[11]);
            
        }

        private double[] addElement(double[] array,double value)
        {
            int pos = array.Length - 1;

            if (array[pos] != 0) {
                Array.Resize(ref array, array.Length + 1);
                array[pos] = value;
            } else {
                array[pos] = value;
            }

            return array;
        }
        */
}
