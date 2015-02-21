VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3150
   ClientLeft      =   60
   ClientTop       =   390
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3150
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows-Standard
   Begin VB.CommandButton Command3 
      Caption         =   "dumprunning"
      Height          =   615
      Left            =   240
      TabIndex        =   3
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Quit"
      Height          =   375
      Left            =   2400
      TabIndex        =   2
      Top             =   360
      Width           =   1095
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Console"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   1200
      Width           =   3495
   End
   Begin VB.CommandButton Command1 
      Caption         =   "dumpskins"
      Height          =   735
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim ad As AveDeskLib.Application

Private Sub Check1_Click()
    If Check1.Value Then
        ad.Debug.Console = True
    Else
        ad.Debug.Console = False
    End If
End Sub

Private Sub Command1_Click()
    Dim ds As DeskletSkin, d As Desklet
    For Each d In ad.AvailableDesklets
        If d.Name Like "Multimeter*" Then
            Debug.Print d.Name
            For Each ds In d.Skins
                Debug.Print "- "; ds.Name
            Next ds
        End If
    Next d
End Sub

Private Sub Command2_Click()
    ad.Close
End Sub

Private Sub Command3_Click()
    Dim d As AveDeskLib.Desklet
    For Each d In ad.RunningDesklets
        Debug.Print d.About.Name, TypeName(d.ObjectModel)
    Next d
End Sub

Private Sub Form_Load()
    Dim d As AveDeskLib.DeskletInfo
    
    Set ad = New Application
    Debug.Print ad.Version
    Debug.Print ad.ExePath
End Sub
