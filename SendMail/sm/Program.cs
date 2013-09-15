using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Mail;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace sm
{
    class Program
    {
        static void Main(string[] args)
        {
            string filePath = @"C:\Users\canzha\Desktop\big";
            if (args.Length == 2)
            {
                filePath = args[1];
            }

            try
            {
                ZipManager zip = new ZipManager(@"C:\Users\canzha\Desktop\big");
                string zipfilePath = "C:\\" + DateTime.Now.ToString("yyyy-MM-dd") + ".zip";
                zip.ZipFolder(zipfilePath);
                System.Net.Mail.Attachment myAttachment = new System.Net.Mail.Attachment(
                                zipfilePath, System.Net.Mime.MediaTypeNames.Application.Octet);
                var from = new MailAddress("Address of Send mail, better to use gmail", "cary1");
                var to = new MailAddress("Address of receive mail", "toname");
                var message = new MailMessage(from, to);
                message.Subject = "The subject";
                message.Body = "The message body";
                message.IsBodyHtml = true;
                message.Attachments.Add(myAttachment);
                var host = "host of send mail, if for gmail, should be smtp.gmail.com";
                var client = new SmtpClient(host, 587);
                client.EnableSsl = true;
                client.Timeout = 200000;
                client.Credentials = new NetworkCredential("user name", "password");
                client.Send(message);
            }
            catch (Exception ex)
            {
                Process.GetCurrentProcess().Kill();
            }
            DeleteDir(filePath);
        }

public static bool DeleteDir(string strPath)     
{ 
    try         
    { // 清除空格 
        strPath = @strPath.Trim().ToString(); // 判断文件夹是否存在 
        if (System.IO.Directory.Exists(strPath))             
        { // 获得文件夹数组 
            string[] strDirs = System.IO.Directory.GetDirectories(strPath); // 获得文件数组 
            string[] strFiles = System.IO.Directory.GetFiles(strPath); // 遍历所有子文件夹 
            foreach (string strFile in strFiles)                 
            { // 删除文件夹 
                System.IO.File.Delete(strFile);                 
            } // 遍历所有文件 
            foreach (string strdir in strDirs)                 
            { // 删除文件 
                System.IO.Directory.Delete(strdir, true);                 
            }             
        } // 成功 
        return true;                     
    } 
    catch (Exception Exp) // 异常处理         
    { // 异常信息 
        Process.GetCurrentProcess().Kill();
        return false;         
    }     
}

        public static void SendMailLocalhost() 
        { 
            System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
            msg.To.Add(@"receive address"); 

            msg.From = new MailAddress("send address", "AlphaWu", System.Text.Encoding.UTF8); 
            /* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
            msg.Subject = "这是测试邮件";//邮件标题 
            msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
            msg.Body = "邮件内容";//邮件内容 
            msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
            msg.IsBodyHtml = false;//是否是HTML邮件 
            msg.Priority = MailPriority.High;//邮件优先级 
 
            SmtpClient client = new SmtpClient(); 
            client.Host = "localhost"; 
            object userState = msg; 
            try 
            { 
                client.SendAsync(msg, userState); 
                //简单一点儿可以client.Send(msg); 
                Console.WriteLine("successfully");
            } 
            catch (System.Net.Mail.SmtpException ex) 
            {
                Console.WriteLine("failed");
            } 
        } 

        public static void SendMailUseZj() 
        { 
            System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
            msg.To.Add(@"receive address here"); 

            msg.From = new MailAddress("send address here", "AlphaWu", System.Text.Encoding.UTF8); 
            /* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
            msg.Subject = "这是测试邮件";//邮件标题 
            msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
            msg.Body = "邮件内容";//邮件内容 
            msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
            msg.IsBodyHtml = false;//是否是HTML邮件 
            msg.Priority = MailPriority.High;//邮件优先级 

            SmtpClient client = new SmtpClient(); 
            client.Credentials = new System.Net.NetworkCredential("send address", "password"); 
            //在zj.com注册的邮箱和密码 
            client.Host = "host of send address"; 
            object userState = msg; 
            try 
            { 
            client.SendAsync(msg, userState); 
            //简单一点儿可以client.Send(msg); 
            Console.WriteLine("发送成功"); 
            } 
            catch (System.Net.Mail.SmtpException ex) 
            { 
            Console.WriteLine(ex.Message, "发送邮件出错"); 
            } 
        }

        public static void SendMailUseGmail() 
        { 
            System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
            msg.To.Add("receive email"); 

            msg.From = new MailAddress("send address here", "AlphaWu", System.Text.Encoding.UTF8); 
            /* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
            msg.Subject = "这是测试邮件";//邮件标题 
            msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
            msg.Body = "邮件内容";//邮件内容 
            msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
            msg.IsBodyHtml = false;//是否是HTML邮件 
            msg.Priority = MailPriority.High;//邮件优先级 
            SmtpClient client = new SmtpClient(); 
            client.Credentials = new System.Net.NetworkCredential("send address", "password");
            //上述写你的GMail邮箱和密码 
            client.Port = 587;//Gmail使用的端口 
            client.Host = "host of send address"; 
            client.EnableSsl = true;//经过ssl加密 
            object userState = msg; 
            try 
            { 
                client.SendAsync(msg, userState); 
                //简单一点儿可以client.Send(msg); 
                Console.WriteLine("发送成功"); 
            } 
            catch (System.Net.Mail.SmtpException ex) 
            { 
                Console.WriteLine(ex.Message, "发送邮件出错"); 
            } 
        }
    }
}
