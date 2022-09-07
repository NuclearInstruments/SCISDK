using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Oscilloscope
{
    // class used to plot and manage oscilloscope output graph
    class OscilloscopeGraph
    {
        PlotView plot_view = null;
        LinearAxis x_axis;
        LinearAxis y_axis;

        public OscilloscopeGraph(Point position, Size size)
        {
            // create a PlotView type object (used to plot graph) and set position and size
            plot_view = new PlotView();
            plot_view.Location = position;
            plot_view.Size = size;

            // initialize axis
            x_axis = new LinearAxis { Position = AxisPosition.Bottom, Title = "" };
            y_axis = new LinearAxis { Position = AxisPosition.Left, Title = "" };

            plot_view.Model = new PlotModel();

            // add axis to model
            plot_view.Model.Axes.Add(x_axis);
            plot_view.Model.Axes.Add(y_axis);
            
            plot_view.Anchor = (AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top);
        }

        // method used to repaint graph
        public void Refresh()
        {
            plot_view.Refresh();
        }

        public void SetSize(Size size)
        {
            plot_view.Size = size;
            //plot_view.Invalidate(true);
        }

        public void SetMaxValueY(double value)
        {
            y_axis.Maximum = value;
            //plot_view.Invalidate(true);
        }

        public void SetMinValueY(double value)
        {
            y_axis.Minimum = value;
            //plot_view.Invalidate(true);
        }

        public void SetMaxValueX(double value)
        {
            x_axis.Maximum = value;
            //plot_view.Invalidate(true);
        }

        public PlotModel GetModel()
        {
            return plot_view.Model;
        }

        public void SetMinValueX(double value)
        {
            x_axis.Minimum = value;
            //plot_view.Invalidate(true);
        }

        //method used to enable autoscale on y axis
        public void EnableAutoscaleY()
        {
            y_axis.Reset();
        }

        // method used to add the graph on the UI
        public void AddToUI(Control container)
        {
            container.Controls.Add(plot_view);
        }

        // method used to set x axis label
        public void SetXAxisLabel(string txt)
        {
            x_axis.Title = txt;
        }

        // method used to set y axis label
        public void SetYAxisLabel(string txt)
        {
            y_axis.Title = txt;
        }

        public void XAxisVisible(bool visibile)
        {
            x_axis.IsAxisVisible = visibile;
        }



    }
}
