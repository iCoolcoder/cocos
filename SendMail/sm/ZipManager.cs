using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Packaging;


namespace sm
{
    class ZipManager
    {

        public string SourceFolderPath { get; set; }

        public ZipManager(string sourceFolderPath)
        {
            SourceFolderPath = sourceFolderPath;
        }

        public void ZipFolder(string zipFilePath)
        {
            using (Package package = Package.Open(zipFilePath, System.IO.FileMode.Create))
            {
                DirectoryInfo di = new DirectoryInfo(SourceFolderPath);
                ZipDirectory(di, package);

            }

            
        }

        private void ZipDirectory(DirectoryInfo di, Package package)
        {
            foreach (FileInfo fi in di.GetFiles())
            {
                string relativePath = fi.FullName.Replace(SourceFolderPath, String.Empty);
                relativePath = relativePath.Replace("\\","/");

                PackagePart part = package.CreatePart(new Uri(relativePath, UriKind.Relative), System.Net.Mime.MediaTypeNames.Application.Zip);

                using (FileStream fs = fi.OpenRead())
                {
                    CopyStream(fs, part.GetStream());
                }
            }

            foreach (DirectoryInfo subDi in di.GetDirectories())
            {
                ZipDirectory(subDi, package);
            }
        }

        private void CopyStream(Stream source, Stream target)
        {
            const int bufSize = 0x1000;
            byte[] buf = new byte[bufSize];
            int bytesRead = 0;
            while ((bytesRead = source.Read(buf, 0, bufSize)) > 0)
            {
                target.Write(buf, 0, bytesRead);
            }
        }
    }
}
