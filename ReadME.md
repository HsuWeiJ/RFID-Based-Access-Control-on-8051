# RFID Based Access Control

在這個專案當中，我使用8051MCU構建一個RFID門禁管理系統，如果您去過大醫院或公司，您可能已經看到使用 RFID 的門禁卡來限制對某些區域的訪問。在醫院中，醫院工作人員需獲得批准的 RFID 標籤才能進入重症監護室或此等限制區域。

## Project ****Description****

利用RFID進行門禁管理時，需要在系統中設定獲得批准的RFID標籤，當擁有正確的RFID 卡（與系統中預加載的數據相同）的人來刷他的 RFID門禁卡時，將授予訪問權限。當持有錯誤 RFID 卡的人（其數據未加載到系統中）刷他的 RFID門禁卡時，訪問將被拒絕。

![image](https://github.com/HsuWeiJ/InterviewSideProject/blob/master/RFID_LCD_Project/RFID_Based_Access_Control_using_8051.png)

- 上圖為 RFID 模組連接到 8051 的電路圖。
- RFID 門禁卡中的 ID將由程式讀取並顯示在 16×2 LCD 顯示屏上。RFID 模組的 Tx 接腳連接到8051的P3_5 (RX)。8051通過該接腳從 RFID 模組接收數據。
- 開關S1、電容C1和電阻R1為Reset電路。
- 電容 C2、C3 和晶體 X1 為外部時鐘源，我使用的晶振頻率為12MHz，在設計UART的鮑率時需注意。

## Files

- RFID_System.c
    - UART的鮑率設定、定時器設定、中斷設定
    - LCD模組的初始化
    - RFID系統的演算法
- regx52.h
    - 8051的標準函式庫，包含special funtion register

## Notes

- [Notion](https://zippy-stretch-eb0.notion.site/8051-RFID-Access-control-2c67e968bec74822b4e508a5a055926b "link")
- [PDF](https://drive.google.com/file/d/1LVrrkECrpcI217pk-Pp_OPU__6wGkkzl/view?usp=sharing "link") (Notion匯出的版本，可能會跑板)
