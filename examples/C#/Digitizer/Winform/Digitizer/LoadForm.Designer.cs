namespace Digitizer
{
    partial class LoadForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.cmb_model = new System.Windows.Forms.ComboBox();
            this.txt_json_file = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_browse = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.cmb_connection_type = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.txt_sn = new System.Windows.Forms.TextBox();
            this.txt_ip = new System.Windows.Forms.TextBox();
            this.btn_connect = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(108, 229);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Model";
            // 
            // cmb_model
            // 
            this.cmb_model.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_model.FormattingEnabled = true;
            this.cmb_model.Location = new System.Drawing.Point(195, 225);
            this.cmb_model.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.cmb_model.Name = "cmb_model";
            this.cmb_model.Size = new System.Drawing.Size(180, 28);
            this.cmb_model.TabIndex = 1;
            this.cmb_model.SelectedIndexChanged += new System.EventHandler(this.cmb_model_SelectedIndexChanged);
            // 
            // txt_json_file
            // 
            this.txt_json_file.Location = new System.Drawing.Point(195, 266);
            this.txt_json_file.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txt_json_file.Name = "txt_json_file";
            this.txt_json_file.Size = new System.Drawing.Size(400, 26);
            this.txt_json_file.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(86, 271);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(75, 20);
            this.label2.TabIndex = 3;
            this.label2.Text = "JSON file";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btn_browse
            // 
            this.btn_browse.Location = new System.Drawing.Point(606, 263);
            this.btn_browse.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_browse.Name = "btn_browse";
            this.btn_browse.Size = new System.Drawing.Size(112, 35);
            this.btn_browse.TabIndex = 4;
            this.btn_browse.Text = "Browse";
            this.btn_browse.UseVisualStyleBackColor = true;
            this.btn_browse.Click += new System.EventHandler(this.btn_browse_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(50, 311);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(124, 20);
            this.label3.TabIndex = 6;
            this.label3.Text = "Connection type";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmb_connection_type
            // 
            this.cmb_connection_type.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_connection_type.FormattingEnabled = true;
            this.cmb_connection_type.Location = new System.Drawing.Point(195, 306);
            this.cmb_connection_type.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.cmb_connection_type.Name = "cmb_connection_type";
            this.cmb_connection_type.Size = new System.Drawing.Size(180, 28);
            this.cmb_connection_type.TabIndex = 7;
            this.cmb_connection_type.SelectedIndexChanged += new System.EventHandler(this.cmb_connection_type_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(412, 315);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(107, 20);
            this.label4.TabIndex = 8;
            this.label4.Text = "Serial number";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(464, 355);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(64, 20);
            this.label5.TabIndex = 9;
            this.label5.Text = "IP : port";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txt_sn
            // 
            this.txt_sn.Location = new System.Drawing.Point(528, 311);
            this.txt_sn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txt_sn.Name = "txt_sn";
            this.txt_sn.Size = new System.Drawing.Size(188, 26);
            this.txt_sn.TabIndex = 10;
            // 
            // txt_ip
            // 
            this.txt_ip.Location = new System.Drawing.Point(528, 351);
            this.txt_ip.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txt_ip.Name = "txt_ip";
            this.txt_ip.Size = new System.Drawing.Size(188, 26);
            this.txt_ip.TabIndex = 11;
            // 
            // btn_connect
            // 
            this.btn_connect.Location = new System.Drawing.Point(346, 429);
            this.btn_connect.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_connect.Name = "btn_connect";
            this.btn_connect.Size = new System.Drawing.Size(112, 35);
            this.btn_connect.TabIndex = 12;
            this.btn_connect.Text = "Connect";
            this.btn_connect.UseVisualStyleBackColor = true;
            this.btn_connect.Click += new System.EventHandler(this.btn_connect_Click);
            // 
            // LoadForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 503);
            this.Controls.Add(this.btn_connect);
            this.Controls.Add(this.txt_ip);
            this.Controls.Add(this.txt_sn);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cmb_connection_type);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btn_browse);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txt_json_file);
            this.Controls.Add(this.cmb_model);
            this.Controls.Add(this.label1);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "LoadForm";
            this.Text = "Oscilloscope dual - open connection";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmb_model;
        private System.Windows.Forms.TextBox txt_json_file;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_browse;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cmb_connection_type;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txt_sn;
        private System.Windows.Forms.TextBox txt_ip;
        private System.Windows.Forms.Button btn_connect;
    }
}