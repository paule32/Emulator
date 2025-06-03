unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ComCtrls, ExtCtrls,
  DBCtrls, Menus, StdCtrls, EditBtn, ComboEx, Buttons, ExtendedTabControls,
  laz.VirtualTrees, laz.VTHeaderPopup, RTTIGrids, RTTICtrls,
  SynHighlighterMulti, SynEdit, FileUtil;

type

  { TForm1 }

  TForm1 = class(TForm)
    ComboBoxEx2: TComboBoxEx;
    ComboBoxEx3: TComboBoxEx;
    CoolBar1: TCoolBar;
    CoolBar2: TCoolBar;
    CoolBar3: TCoolBar;
    EditButton2: TEditButton;
    EditButton3: TEditButton;
    ImageList1: TImageList;
    ListBox1: TListBox;
    MainMenu1: TMainMenu;
    MenuItem1: TMenuItem;
    MenuItem10: TMenuItem;
    MenuItem2: TMenuItem;
    MenuItem3: TMenuItem;
    MenuItem4: TMenuItem;
    MenuItem5: TMenuItem;
    MenuItem6: TMenuItem;
    MenuItem7: TMenuItem;
    MenuItem8: TMenuItem;
    MenuItem9: TMenuItem;
    MultiPropertyLink1: TMultiPropertyLink;
    PageControl2: TPageControl;
    PageControl3: TPageControl;
    PageControl4: TPageControl;
    Panel4: TPanel;
    Panel5: TPanel;
    Panel6: TPanel;
    Separator2: TMenuItem;
    Separator1: TMenuItem;
    Panel1: TPanel;
    Panel2: TPanel;
    Panel3: TPanel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    SpeedButton8: TSpeedButton;
    Splitter1: TSplitter;
    Splitter2: TSplitter;
    Splitter3: TSplitter;
    StatusBar1: TStatusBar;
    SynMultiSyn1: TSynMultiSyn;
    TabControl1: TPageControl;
    TabSheet4: TTabSheet;
    TabSheet5: TTabSheet;
    TabSheet6: TTabSheet;
    TIPropertyGrid1: TTIPropertyGrid;
    TreeView2: TTreeView;
    TreeView3: TTreeView;
    procedure FormShow(Sender: TObject);
    procedure MenuItem1Click(Sender: TObject);
    procedure PageControl1Change(Sender: TObject);
    procedure TabControl1Change(Sender: TObject);
    procedure TreeView3DblClick(Sender: TObject);
  private
    openFileList: TStringList;
  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.TabControl1Change(Sender: TObject);
begin

end;

procedure TForm1.TreeView3DblClick(Sender: TObject);
var
  tab: TTabSheet;
  syn: TSynEdit;
begin
  if TreeView3.Selected <> nil then
  begin
    if not Assigned(openFileList) then
    OpenFileList := TStringList.Create;

    if OpenFileList.IndexOf(TreeView3.Selected.Text) > -1 then
    begin
      ShowMessage('file is already open.');
      exit;
    end;
    OpenFileList.Add(TreeView3.Selected.Text);

    tab := TTabSheet.Create(PageControl2);
    tab.PageControl := PageControl2;
    tab.Caption := TreeView3.Selected.Text;

    syn := TSynEdit.Create(tab);
    syn.Parent := tab;
    syn.Align  := alClient;
  end;
end;

procedure TForm1.MenuItem1Click(Sender: TObject);
begin

end;

procedure AddFilesToNode(tree: TTreeView; ParentNode: TTreeNode; const Directory, Extension: string);
var
  SearchRec: TSearchRec;
  FileNode: TTreeNode;
  FullPath: String;
begin
  if ParentNode = nil then Exit;

  tree.Items.BeginUpdate;
  try
    // Dateien suchen
    FullPath := IncludeTrailingPathDelimiter(Directory);
    if FindFirst(FullPath + '*.' + Extension, faAnyFile and not faDirectory, SearchRec) = 0 then
    begin
      repeat
        FileNode := Tree.Items.AddChild(ParentNode, SearchRec.Name);
        // Optional: Tag oder Data zuordnen, z.B. voller Pfad
        FileNode.Data := Pointer(StrNew(PChar(FullPath + SearchRec.Name)));
      until FindNext(SearchRec) <> 0;
      FindClose(SearchRec);
    end;
  finally
    Tree.Items.EndUpdate;
  end;
end;


procedure TForm1.FormShow(Sender: TObject);
var
  srcNode: TTreeNode;
  outNode: TTreeNode;
begin
  srcNode := TreeView3.Items.FindNodeWithText('BIOS Kernel');
  srcNode := srcNode.GetFirstChild;
  outNode := srcNode.GetNext;

  if srcNode <> nil then AddFilesToNode(TreeView3, srcNode, './bios/src', 'asm');
  if outNode <> nil then AddFilesToNode(TreeView3, outNode, './bios/out', 'elf');


  srcNode := TreeView3.Items.FindNodeWithText('MS-DOS Kernel');
  srcNode := srcNode.GetFirstChild;
  outNode := srcNode.GetNext;

  if srcNode <> nil then AddFilesToNode(TreeView3, srcNode, './msdos/src', 'asm');
  if outNode <> nil then AddFilesToNode(TreeView3, outNode, './msdos/out', 'elf');

end;

procedure TForm1.PageControl1Change(Sender: TObject);
begin

end;

end.

