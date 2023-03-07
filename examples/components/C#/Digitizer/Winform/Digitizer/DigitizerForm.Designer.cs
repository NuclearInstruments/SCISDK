namespace Digitizer
{
    partial class DigitizerForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DigitizerForm));
            this.check_displayed_channels = new System.Windows.Forms.CheckedListBox();
            this.btn_start = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.lbl2 = new System.Windows.Forms.Label();
            this.lbl_status = new System.Windows.Forms.Label();
            this.btn_reset_zoom = new System.Windows.Forms.Button();
            this.btn_none = new System.Windows.Forms.Button();
            this.btn_all = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cmb_enabled_ch = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.num_wave_length = new System.Windows.Forms.NumericUpDown();
            this.track_wave_length = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.num_number_of_waves = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.txt_filename = new System.Windows.Forms.TextBox();
            this.btn_select_file = new System.Windows.Forms.Button();
            this.check_store_on_file = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.num_wave_length)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_wave_length)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_number_of_waves)).BeginInit();
            this.SuspendLayout();
            // 
            // check_displayed_channels
            // 
            this.check_displayed_channels.FormattingEnabled = true;
            this.check_displayed_channels.Location = new System.Drawing.Point(18, 18);
            this.check_displayed_channels.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.check_displayed_channels.Name = "check_displayed_channels";
            this.check_displayed_channels.Size = new System.Drawing.Size(322, 319);
            this.check_displayed_channels.TabIndex = 0;
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Transparent;
            this.btn_start.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_start.BackgroundImage")));
            this.btn_start.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_start.Location = new System.Drawing.Point(372, 29);
            this.btn_start.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(52, 54);
            this.btn_start.TabIndex = 1;
            this.btn_start.UseVisualStyleBackColor = false;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.btn_stop.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_stop.BackgroundImage")));
            this.btn_stop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_stop.Location = new System.Drawing.Point(440, 29);
            this.btn_stop.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(52, 54);
            this.btn_stop.TabIndex = 2;
            this.btn_stop.UseVisualStyleBackColor = false;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // lbl2
            // 
            this.lbl2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lbl2.AutoSize = true;
            this.lbl2.Location = new System.Drawing.Point(1192, 48);
            this.lbl2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl2.Name = "lbl2";
            this.lbl2.Size = new System.Drawing.Size(72, 20);
            this.lbl2.TabIndex = 6;
            this.lbl2.Text = "STATUS";
            // 
            // lbl_status
            // 
            this.lbl_status.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lbl_status.AutoSize = true;
            this.lbl_status.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.lbl_status.Location = new System.Drawing.Point(1278, 48);
            this.lbl_status.Margin = new System.Windows.Forms.Padding(0);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(46, 20);
            this.lbl_status.TabIndex = 7;
            this.lbl_status.Text = "IDLE";
            // 
            // btn_reset_zoom
            // 
            this.btn_reset_zoom.Location = new System.Drawing.Point(501, 40);
            this.btn_reset_zoom.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_reset_zoom.Name = "btn_reset_zoom";
            this.btn_reset_zoom.Size = new System.Drawing.Size(112, 35);
            this.btn_reset_zoom.TabIndex = 22;
            this.btn_reset_zoom.Text = "Reset zoom";
            this.btn_reset_zoom.UseVisualStyleBackColor = true;
            this.btn_reset_zoom.Click += new System.EventHandler(this.btn_reset_zoom_Click);
            // 
            // btn_none
            // 
            this.btn_none.Location = new System.Drawing.Point(268, 357);
            this.btn_none.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_none.Name = "btn_none";
            this.btn_none.Size = new System.Drawing.Size(74, 35);
            this.btn_none.TabIndex = 23;
            this.btn_none.Text = "NONE";
            this.btn_none.UseVisualStyleBackColor = true;
            this.btn_none.Click += new System.EventHandler(this.btn_none_Click);
            // 
            // btn_all
            // 
            this.btn_all.Location = new System.Drawing.Point(186, 357);
            this.btn_all.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_all.Name = "btn_all";
            this.btn_all.Size = new System.Drawing.Size(74, 35);
            this.btn_all.TabIndex = 24;
            this.btn_all.Text = "ALL";
            this.btn_all.UseVisualStyleBackColor = true;
            this.btn_all.Click += new System.EventHandler(this.btn_all_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 415);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(136, 20);
            this.label1.TabIndex = 25;
            this.label1.Text = "Enabled channels";
            // 
            // cmb_enabled_ch
            // 
            this.cmb_enabled_ch.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_enabled_ch.FormattingEnabled = true;
            this.cmb_enabled_ch.Location = new System.Drawing.Point(165, 411);
            this.cmb_enabled_ch.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.cmb_enabled_ch.Name = "cmb_enabled_ch";
            this.cmb_enabled_ch.Size = new System.Drawing.Size(180, 28);
            this.cmb_enabled_ch.TabIndex = 26;
            this.cmb_enabled_ch.SelectedIndexChanged += new System.EventHandler(this.cmb_enabled_ch_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 462);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 20);
            this.label2.TabIndex = 27;
            this.label2.Text = "Wave length";
            // 
            // num_wave_length
            // 
            this.num_wave_length.Location = new System.Drawing.Point(162, 458);
            this.num_wave_length.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.num_wave_length.Maximum = new decimal(new int[] {
            922,
            0,
            0,
            0});
            this.num_wave_length.Name = "num_wave_length";
            this.num_wave_length.Size = new System.Drawing.Size(180, 26);
            this.num_wave_length.TabIndex = 28;
            this.num_wave_length.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.num_wave_length.ValueChanged += new System.EventHandler(this.num_wave_length_ValueChanged);
            // 
            // track_wave_length
            // 
            this.track_wave_length.Location = new System.Drawing.Point(18, 503);
            this.track_wave_length.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.track_wave_length.Maximum = 922;
            this.track_wave_length.Name = "track_wave_length";
            this.track_wave_length.Size = new System.Drawing.Size(328, 69);
            this.track_wave_length.SmallChange = 50;
            this.track_wave_length.TabIndex = 29;
            this.track_wave_length.TickFrequency = 50;
            this.track_wave_length.Value = 130;
            this.track_wave_length.Scroll += new System.EventHandler(this.track_wave_length_Scroll);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 577);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(131, 20);
            this.label3.TabIndex = 30;
            this.label3.Text = "Number of waves";
            // 
            // num_number_of_waves
            // 
            this.num_number_of_waves.Location = new System.Drawing.Point(162, 574);
            this.num_number_of_waves.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.num_number_of_waves.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_number_of_waves.Name = "num_number_of_waves";
            this.num_number_of_waves.Size = new System.Drawing.Size(180, 26);
            this.num_number_of_waves.TabIndex = 31;
            this.num_number_of_waves.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_number_of_waves.ValueChanged += new System.EventHandler(this.num_number_of_waves_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(18, 626);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(74, 20);
            this.label4.TabIndex = 32;
            this.label4.Text = "Filename";
            // 
            // txt_filename
            // 
            this.txt_filename.Location = new System.Drawing.Point(100, 622);
            this.txt_filename.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txt_filename.Name = "txt_filename";
            this.txt_filename.Size = new System.Drawing.Size(184, 26);
            this.txt_filename.TabIndex = 33;
            this.txt_filename.Text = "c:\\temp\\abc.txt";
            // 
            // btn_select_file
            // 
            this.btn_select_file.Location = new System.Drawing.Point(292, 618);
            this.btn_select_file.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_select_file.Name = "btn_select_file";
            this.btn_select_file.Size = new System.Drawing.Size(50, 35);
            this.btn_select_file.TabIndex = 34;
            this.btn_select_file.Text = "...";
            this.btn_select_file.UseVisualStyleBackColor = true;
            this.btn_select_file.Click += new System.EventHandler(this.btn_select_file_Click);
            // 
            // check_store_on_file
            // 
            this.check_store_on_file.AutoSize = true;
            this.check_store_on_file.Location = new System.Drawing.Point(22, 662);
            this.check_store_on_file.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.check_store_on_file.Name = "check_store_on_file";
            this.check_store_on_file.Size = new System.Drawing.Size(120, 24);
            this.check_store_on_file.TabIndex = 35;
            this.check_store_on_file.Text = "Store on file";
            this.check_store_on_file.UseVisualStyleBackColor = true;
            // 
            // DigitizerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1528, 885);
            this.Controls.Add(this.check_store_on_file);
            this.Controls.Add(this.btn_select_file);
            this.Controls.Add(this.txt_filename);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.num_number_of_waves);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.track_wave_length);
            this.Controls.Add(this.num_wave_length);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cmb_enabled_ch);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_all);
            this.Controls.Add(this.btn_none);
            this.Controls.Add(this.btn_reset_zoom);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.lbl2);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.check_displayed_channels);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "DigitizerForm";
            this.Text = "Oscilloscope";
            this.Load += new System.EventHandler(this.OscilloscopeForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.num_wave_length)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_wave_length)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_number_of_waves)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox check_displayed_channels;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Label lbl2;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Button btn_reset_zoom;
        private System.Windows.Forms.Button btn_none;
        private System.Windows.Forms.Button btn_all;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmb_enabled_ch;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown num_wave_length;
        private System.Windows.Forms.TrackBar track_wave_length;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown num_number_of_waves;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txt_filename;
        private System.Windows.Forms.Button btn_select_file;
        private System.Windows.Forms.CheckBox check_store_on_file;
    }
}

