#include "stdafx.h"
#include "CSendMail.h"
#include <afxwin.h>
#include <atlimage.h>
#include <MAPI.h>

HANDLE DDBtoDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE  hPal,DWORD * sizeimage) ;
BOOL CapScreen(LPTSTR FileName);//截屏函数
void CreateStretchImage(CImage *pImage,CImage *ResultImage,char *FileName, int StretchHeight,int StretchWidth);
void ChangeSize(char *img1Path, char *img3Path)
{
	CImage img1, img3;  
  
	img1.Load(img1Path);   

	CBitmap bmp;  
	CDC memDC;  
	HDC hDC=NULL;  
	CDC *pDC=NULL;  
	CBitmap *pOld=NULL;  
	HBITMAP hBitmap=NULL;  
  
	//创建位图  
	hDC=img1.GetDC();  
	pDC=CDC::FromHandle(hDC);  
	bmp.DeleteObject();  
	bmp.CreateCompatibleBitmap(pDC,img1.GetWidth()/2,img1.GetHeight());  
	memDC.DeleteDC();  
	memDC.CreateCompatibleDC(pDC);  
	pOld=memDC.SelectObject(&bmp);  
  
	::SetStretchBltMode(memDC.m_hDC,HALFTONE);  
	::SetBrushOrgEx(memDC.m_hDC,0,0,NULL);  
  
	// 背景置白色  
	CRgn rectRgn;  
	rectRgn.CreateRectRgn(0,0,img1.GetWidth()/8,img1.GetHeight()/8);  
	CBrush brush;  
	brush.CreateSolidBrush(RGB(255,255,255));  
	memDC.FillRgn(&rectRgn,&brush);     
	//画图  
	img1.StretchBlt(memDC.m_hDC,CRect(0,0,img1.GetWidth()/8,img1.GetHeight()/8),CRect(0,0,img1.GetWidth(),img1.GetHeight()),SRCCOPY);  
	// img2.StretchBlt(memDC.m_hDC,CRect(0,img1.GetHeight()/2,img1.GetWidth()/2,img1.GetHeight()),CRect(0,0,img2.GetWidth(),img2.GetHeight()),SRCCOPY); 	
  
	hBitmap=(HBITMAP)memDC.SelectObject(pOld->m_hObject);  
	
	img3.Attach(hBitmap);// 载入位图资源
	
	img3.Save(img3Path); // 保存新的位图  
  
	img1.ReleaseDC();  
	img1.Destroy();  
	img3.Destroy(); 
}

void CreateStretchImage(CImage *pImage,CImage *ResultImage,char *FileName, int StretchHeight,int StretchWidth){

    if(pImage->IsDIBSection()){

          // 取得 pImage 的 DC
	  CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image 因為有自己的 DC, 所以必須使用 FromHandle 取得對應的 DC

	  CBitmap *bitmap1=pImageDC1->GetCurrentBitmap();
	  BITMAP bmpInfo;

	  bitmap1->GetBitmap(&bmpInfo);

	  // 建立新的 CImage
	  ResultImage->Create(StretchWidth,StretchHeight,bmpInfo.bmBitsPixel);

	  CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());
	  // 當 Destination 比較小的時候, 會根據 Destination DC 上的 Stretch Blt mode 決定是否要保留被刪除點的資訊

	  ResultImageDC->SetStretchBltMode(HALFTONE); // 使用最高品質的方式

	  ::SetBrushOrgEx(ResultImageDC->m_hDC,0,0,NULL); // 調整 Brush 的起點

	  // 把 pImage 畫到 ResultImage 上面
	  StretchBlt(*ResultImageDC,0,0,StretchWidth,StretchHeight,*pImageDC1,0,0,pImage->GetWidth(),pImage->GetHeight(),SRCCOPY);
	  ResultImage->Save(FileName);
	  pImage->ReleaseDC();
	  ResultImage->ReleaseDC();

     }

}


HANDLE DDBtoDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE  hPal,DWORD * sizeimage)
{
    BITMAP            bm;
    BITMAPINFOHEADER    bi;
     LPBITMAPINFOHEADER     lpbi;
    DWORD            dwLen;
    HANDLE            hDib;
    HANDLE            handle;
    HDC             hdc;

    //不支持BI_BITFIELDS类型
    if( dwCompression == BI_BITFIELDS )
        return NULL;

    //如果调色板为空，则用默认调色板
    if (hPal==NULL)
        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE );

    //获取位图信息
    GetObject(bitmap,sizeof(bm),(LPSTR)&bm);

    //初始化位图信息头
    bi.biSize        = sizeof(BITMAPINFOHEADER);
    bi.biWidth        = bm.bmWidth;
    bi.biHeight         = bm.bmHeight;
    bi.biPlanes         = 1;
    bi.biBitCount        = bm.bmPlanes * bm.bmBitsPixel;
    bi.biCompression    = dwCompression;
    bi.biSizeImage        = 0;
    bi.biXPelsPerMeter    = 0;
    bi.biYPelsPerMeter    = 0;
    bi.biClrUsed        = 0;
    bi.biClrImportant    = 0;

    //计算信息头及颜色表大小
    int ncolors = (1 << bi.biBitCount); if( ncolors> 256 )
        ncolors = 0;
    dwLen  = bi.biSize + ncolors * sizeof(RGBQUAD);

    // we need a device context to get the dib from
    hdc = GetDC(NULL);
    hPal = SelectPalette(hdc,hPal,FALSE);
    RealizePalette(hdc);

    //为信息头及颜色表分配内存
    hDib = GlobalAlloc(GMEM_FIXED,dwLen);

    if (!hDib){
        SelectPalette(hdc,hPal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDib;
    *lpbi = bi;
    //调用 GetDIBits 计算图像大小
    GetDIBits(hdc, bitmap, 0L, (DWORD)bi.biHeight,
            (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS );

    bi = *lpbi;
    //图像的每一行都对齐(32bit)边界
    if (bi.biSizeImage == 0){
        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
                        * bi.biHeight;
    if (dwCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    //重新分配内存大小，以便放下所有数据
    dwLen += bi.biSizeImage;
    if (handle = GlobalReAlloc(hDib, dwLen, GMEM_MOVEABLE))
        hDib = handle;
    else{
        GlobalFree(hDib);

        //重选原始调色板
        SelectPalette(hdc,hPal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    //获取位图数据
    lpbi = (LPBITMAPINFOHEADER)hDib;

    //最终获得的DIB
    BOOL bgotbits = GetDIBits( hdc, bitmap,
                  0L,                      //扫描行起始处
                (DWORD)bi.biHeight,      //扫描行数
                (LPBYTE)lpbi             //位图数据地址
                + (bi.biSize + ncolors * sizeof(RGBQUAD)),
                (LPBITMAPINFO)lpbi,      //位图信息地址
                (DWORD)DIB_RGB_COLORS);  //颜色板使用RGB

    if( !bgotbits )
    {
        GlobalFree(hDib);
        
        SelectPalette(hdc,hPal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    SelectPalette(hdc,hPal,FALSE);
    ReleaseDC(NULL,hdc);
    *sizeimage=bi.biSizeImage;
    return hDib;
}

BOOL CapScreen(LPTSTR tmpFileName)
{
    DWORD sizeimage;
    HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
    HDC CompatibleHDC = CreateCompatibleDC(hdc);
    HBITMAP BmpScreen = CreateCompatibleBitmap(hdc,GetDeviceCaps(hdc, HORZRES),GetDeviceCaps(hdc, VERTRES));
    SelectObject(CompatibleHDC, BmpScreen);
    BitBlt(CompatibleHDC,0,0,GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES),hdc,0,0,SRCCOPY);
    

	HANDLE    pbitmapwithoutfileh = DDBtoDIB(BmpScreen, BI_RGB,0,&sizeimage);
    BITMAPFILEHEADER bfh;
   //设置位图信息头结构
    bfh.bfType = ((WORD)('M'<< 8)|'B');
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 54+sizeimage;
    bfh.bfOffBits = 54;
    //创建位图文件    
	HANDLE  hFile=CreateFile(tmpFileName,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	DWORD dwWrite;
	// 写入位图文件头
	WriteFile(hFile,&bfh,sizeof(BITMAPFILEHEADER),&dwWrite,NULL);
	// 写入位图文件其余内容
	WriteFile(hFile,pbitmapwithoutfileh,bfh.bfSize,&dwWrite,NULL);

	CloseHandle(hFile);
	DeleteDC(hdc);
   
	CloseHandle(CompatibleHDC);
	return true;
}


int main(int argc, char * argv[])
{
	if ((argc == 2))
	{
		char *tmpFileName = "C:\\tmp.bmp";
		CapScreen(tmpFileName);
		CImage *p1 = new CImage();
		CImage *p2 = new CImage();
		p1->Load(tmpFileName);
		CreateStretchImage(p1, p2, argv[1], 100, 100);
	}
	return 0;
}