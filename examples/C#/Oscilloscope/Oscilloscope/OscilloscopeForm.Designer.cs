namespace Oscilloscope
{
    partial class OscilloscopeForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OscilloscopeForm));
            this.check_lst_channels = new System.Windows.Forms.CheckedListBox();
            this.btn_start = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cmb_data_type = new System.Windows.Forms.ComboBox();
            this.check_autoscale = new System.Windows.Forms.CheckBox();
            this.lbl2 = new System.Windows.Forms.Label();
            this.lbl_status = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txt_horizontal_divisions = new System.Windows.Forms.TextBox();
            this.track_horizontal_divisions = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.track_horizontal_divisions)).BeginInit();
            this.SuspendLayout();
            // 
            // check_lst_channels
            // 
            this.check_lst_channels.FormattingEnabled = true;
            this.check_lst_channels.Location = new System.Drawing.Point(12, 12);
            this.check_lst_channels.Name = "check_lst_channels";
            this.check_lst_channels.Size = new System.Drawing.Size(216, 214);
            this.check_lst_channels.TabIndex = 0;
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Transparent;
            this.btn_start.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_start.BackgroundImage")));
            this.btn_start.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_start.Location = new System.Drawing.Point(248, 19);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(35, 35);
            this.btn_start.TabIndex = 1;
            this.btn_start.UseVisualStyleBackColor = false;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.btn_stop.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_stop.BackgroundImage")));
            this.btn_stop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_stop.Location = new System.Drawing.Point(293, 19);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(35, 35);
            this.btn_stop.TabIndex = 2;
            this.btn_stop.UseVisualStyleBackColor = false;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(396, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Data type";
            // 
            // cmb_data_type
            // 
            this.cmb_data_type.FormattingEnabled = true;
            this.cmb_data_type.Items.AddRange(new object[] {
            "Unsigned",
            "Signed"});
            this.cmb_data_type.Location = new System.Drawing.Point(455, 28);
            this.cmb_data_type.Name = "cmb_data_type";
            this.cmb_data_type.Size = new System.Drawing.Size(121, 21);
            this.cmb_data_type.TabIndex = 4;
            // 
            // check_autoscale
            // 
            this.check_autoscale.AutoSize = true;
            this.check_autoscale.Checked = true;
            this.check_autoscale.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_autoscale.Location = new System.Drawing.Point(604, 30);
            this.check_autoscale.Name = "check_autoscale";
            this.check_autoscale.Size = new System.Drawing.Size(73, 17);
            this.check_autoscale.TabIndex = 5;
            this.check_autoscale.Text = "Autoscale";
            this.check_autoscale.UseVisualStyleBackColor = true;
            this.check_autoscale.CheckedChanged += new System.EventHandler(this.check_autoscale_CheckedChanged);
            // 
            // lbl2
            // 
            this.lbl2.AutoSize = true;
            this.lbl2.Location = new System.Drawing.Point(795, 31);
            this.lbl2.Name = "lbl2";
            this.lbl2.Size = new System.Drawing.Size(50, 13);
            this.lbl2.TabIndex = 6;
            this.lbl2.Text = "STATUS";
            // 
            // lbl_status
            // 
            this.lbl_status.AutoSize = true;
            this.lbl_status.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.lbl_status.Location = new System.Drawing.Point(852, 31);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(31, 13);
            this.lbl_status.TabIndex = 7;
            this.lbl_status.Text = "IDLE";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 301);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(93, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Horizontal (ns/div)";
            // 
            // txt_horizontal_divisions
            // 
            this.txt_horizontal_divisions.Location = new System.Drawing.Point(108, 298);
            this.txt_horizontal_divisions.Name = "txt_horizontal_divisions";
            this.txt_horizontal_divisions.Size = new System.Drawing.Size(120, 20);
            this.txt_horizontal_divisions.TabIndex = 9;
            this.txt_horizontal_divisions.Text = "12000";
            this.txt_horizontal_divisions.TextChanged += new System.EventHandler(this.txt_horizontal_divisions_TextChanged);
            // 
            // track_horizontal_divisions
            // 
            this.track_horizontal_divisions.LargeChange = 100;
            this.track_horizontal_divisions.Location = new System.Drawing.Point(12, 318);
            this.track_horizontal_divisions.Maximum = 75000;
            this.track_horizontal_divisions.Minimum = 12000;
            this.track_horizontal_divisions.Name = "track_horizontal_divisions";
            this.track_horizontal_divisions.Size = new System.Drawing.Size(216, 45);
            this.track_horizontal_divisions.SmallChange = 100;
            this.track_horizontal_divisions.TabIndex = 0;
            this.track_horizontal_divisions.Value = 12000;
            this.track_horizontal_divisions.Scroll += new System.EventHandler(this.track_horizontal_divisions_Scroll);
            // 
            // OscilloscopeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1019, 575);
            this.Controls.Add(this.track_horizontal_divisions);
            this.Controls.Add(this.txt_horizontal_divisions);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.lbl2);
            this.Controls.Add(this.check_autoscale);
            this.Controls.Add(this.cmb_data_type);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.check_lst_channels);
            this.Name = "OscilloscopeForm";
            this.Text = "Oscilloscope";
            this.Load += new System.EventHandler(this.OscilloscopeForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.track_horizontal_divisions)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox check_lst_channels;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmb_data_type;
        private System.Windows.Forms.CheckBox check_autoscale;
        private System.Windows.Forms.Label lbl2;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txt_horizontal_divisions;
        private System.Windows.Forms.TrackBar track_horizontal_divisions;
    }
}

