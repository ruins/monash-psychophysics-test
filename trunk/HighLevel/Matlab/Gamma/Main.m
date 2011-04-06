function varargout = Main(varargin)
% MAIN M-file for Main.fig
%      MAIN, by itself, creates a new MAIN or raises the existing
%      singleton*.
%
%      H = MAIN returns the handle to a new MAIN or the handle to
%      the existing singleton*.
%
%      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN.M with the given input arguments.
%
%      MAIN('Property','Value',...) creates a new MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Main

% Last Modified by GUIDE v2.5 03-Apr-2011 10:43:09

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Main_OpeningFcn, ...
                   'gui_OutputFcn',  @Main_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Main is made visible.
function Main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Main (see VARARGIN)

% Choose default command line output for Main
handles.output = hObject;

handles.gammadir = 'C:\matlab\Gamma\';
handles.resultfoldername= 'PEXP_Result';
handles.nonefacefoldername='PEXP_Noneface';
handles.resolution = set_resolution();
handles.facefoldername = 'PEXP_Face';
handles.grayscale = set_grayscale();
handles.listoffiles=[];
handles.res_list=[];
handles.gscale_list=[];
handles.set_list=[];
handles.face_img_format='gif';
handles.noneface_img_format='jpg';
handles.outputformat='jpg';
handles.ms= set_ms();
handles.blankscreen=-1;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Main wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Main_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function [out]=set_resolution()

temp = [];
temp.list= ['4 x 4'; '6 x 6'; '8 x 8';'10x10'; '12x12';'14x14'; '16x16';'20x20';'25x25';'32x32';'64x64'];
temp.value = [4; 6; 8;10; 12;14;16;20; 25; 32; 64];
temp.current=[];
temp.display=[320 320];
out= temp;



function [out]=set_grayscale()

temp = [];
temp.list= ['2 levels';'3 levels';'4 levels'];
temp.value = [2;3;4];

out= temp;

function clean_current_dir()

D= dir;

if size(D,1)>2

    for i = 3:size(D,1)
        
        if ~(D(i).isdir)
        
        delete(D(i).name);
        else
            
            rmdir(D(i).name,'s');
            
        end
        
    end
    
    
end


% --- Executes on button press in pb_init.
function pb_init_Callback(hObject, eventdata, handles)
% hObject    handle to pb_init (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
cd(strcat(handles.gammadir,handles.facefoldername,'\'));
D = dir;

list = '';

for i = 3:length(D)
    
if (D(i).isdir)
    list = strvcat(list,D(i).name);    
    
end
      
end

set(handles.lb_set,'String',list);

set(handles.lb_gscale,'String',handles.grayscale.list);

set(handles.lb_reso,'String',handles.resolution.list);

set(hObject,'Enable','Off');

function [out] = select_resolution(handles)


a = get(handles.lb_reso,'Value');


out=handles.resolution.value(a);

function [out] = select_gscale(handles)


a = get(handles.lb_gscale,'Value');


out=handles.grayscale.value(a);

function [out] = select_set(handles)


a = get(handles.lb_set,'String');
b = get(handles.lb_set,'Value');


out=a(b,:);

function [out]=set_ms()
        
out.current =0;
out.max=100;

% --- Executes on selection change in lb_reso.
function lb_reso_Callback(hObject, eventdata, handles)
% hObject    handle to lb_reso (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lb_reso contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lb_reso


% --- Executes during object creation, after setting all properties.
function lb_reso_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lb_reso (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in lb_set.
function lb_set_Callback(hObject, eventdata, handles)
% hObject    handle to lb_set (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lb_set contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lb_set


% --- Executes during object creation, after setting all properties.
function lb_set_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lb_set (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in lb_gscale.
function lb_gscale_Callback(hObject, eventdata, handles)
% hObject    handle to lb_gscale (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lb_gscale contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lb_gscale


% --- Executes during object creation, after setting all properties.
function lb_gscale_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lb_gscale (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pb_go.
function pb_go_Callback(hObject, eventdata, handles)
% hObject    handle to pb_go (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject,'Enable','Off');
handles.listoffiles=[];

handles.res_list=select_resolution(handles);
handles.gscale_list=select_gscale(handles);
handles.set_list=select_set(handles);

generate_destination(handles);

handles.ms.current=20;
for i = 1:size(handles.set_list,1)

handles.listoffiles = generate_face_series(handles,i);

handles.listoffiles = generate_noneface_series(handles,i);

end

handles.listoffiles = generate_doc(handles);

process_listoffiles(handles);

set(hObject,'Enable','On');
msgbox('All done.');


function [out] = generate_doc(handles)

cd(handles.gammadir);
paramfile=fopen('Parameters.txt','w');

%print exp params here
fprintf(paramfile,'0;%d \r\n',handles.ms.current);

fprintf(paramfile,'1;%d \r\n',handles.blankscreen);


%resolution
L_r = length(handles.res_list);

fprintf(paramfile,'2;%d',L_r);

for i = 1:L_r
    fprintf(paramfile,',%d',handles.res_list(i));
end

fprintf(paramfile,'\r\n');

%gscale
L_g = length(handles.gscale_list);

fprintf(paramfile,'3;%d',L_g);
for i = 1:L_g
    fprintf(paramfile,',%d',handles.gscale_list(i));
end

fprintf(paramfile,'\r\n');

%set
L_s = size(handles.set_list,1);
fprintf(paramfile,'4;%d \r\n',L_s);
for i = 1:L_s
    
    fprintf(paramfile,'%s,%d \r\n',handles.set_list(i,:),i);
       
end

%set file data
set_size = L_r*L_g*2;

new_list =handles.listoffiles;

for i = 1:L_s
    
    fprintf(paramfile,'5;%d,%d \r\n',i,set_size);
    
      temp_list=randperm(set_size);
      
    for j = 1:set_size
        
 index=(i-1)*set_size+j;
 
   new_list(index)=handles.listoffiles((i-1)*set_size+temp_list(j));
    
    fprintf(paramfile,'%d,%d,%d,%d,%d,%s \r\n',index,new_list(index).set,new_list(index).category,new_list(index).res,new_list(index).gscale,new_list(index).name);
    
    end
   
end

fclose(paramfile);
out=new_list;





function process_listoffiles(handles)

cd(strcat(handles.gammadir,handles.resultfoldername,'\'));

for i = 1:size(handles.listoffiles,2)
    
    I = imread(handles.listoffiles(i).source);
    
    if (length(size(I))>2)
    
    I = rgb2gray(I);
  
    end
        
    %val=255/(handles.gscale_list(handles.listoffiles(i).gscale)-1);
           
      %I = round(I/val)*val;

      
    %  I = adjust_thresh(handles,I,i);
    

      
    I = histeq(uint8(I),handles.gscale_list(handles.listoffiles(i).gscale));  
    
          r= [handles.res_list(handles.listoffiles(i).res) handles.res_list(handles.listoffiles(i).res)];
      
     I = imresize(I,r,'bilinear');
            
    I = disp_phosphene(I,handles.resolution.display);
    
     
    processedfilename = strcat(num2str(i),'.',handles.outputformat);
    
     imwrite(uint8(I),strcat(handles.gammadir,handles.resultfoldername,'\',handles.set_list(handles.listoffiles(i).set,:),'\',processedfilename),handles.outputformat); 
     
     
end



function generate_destination(handles)
res = strcat(handles.gammadir,handles.resultfoldername);

cd(res);
clean_current_dir();

for i = 1:size(handles.set_list,1)
cd(res);
mkdir(handles.set_list(i,:));

end

%function [out] = adjust_thresh(handles,in,i)

%mean = mean2(in);
%stdev = std2(in);
%levels = handles.gscale_list(handles.listoffiles(i).gscale);


      
      
      
      


function [lof] = generate_face_series(handles,i)


    cd(strcat(handles.gammadir,handles.facefoldername,'\',handles.set_list(i,:)));
    
    locallist= dir(strcat('*.',handles.face_img_format));
    
    permtable = randperm(size(locallist,1));
    
    for j = 1:size(handles.res_list,1)
    
        for k = 1:size(handles.gscale_list,1)
            
            index=size(handles.listoffiles,2)+1;
            permID=permtable(j+(k-1)*size(handles.res_list,1));
            
            handles.listoffiles(index).source = strcat(handles.gammadir,handles.facefoldername,'\',handles.set_list(i,:),'\',locallist(permID).name);
        
            handles.listoffiles(index).gscale = k;
            
            handles.listoffiles(index).res= j;
        
            handles.listoffiles(index).set = i;
                       
            handles.listoffiles(index).name = locallist(permID).name;
            
            handles.listoffiles(index).category = 1;
        
        end
    end
    


lof=handles.listoffiles;

function [lof] = generate_noneface_series(handles,i)

    cd(strcat(handles.gammadir,handles.nonefacefoldername,'\'));
    
    locallist= dir(strcat('*.',handles.noneface_img_format));
    
    permtable = randperm(size(locallist,1));
    
    for j = 1:size(handles.res_list,1)
    
        for k = 1:size(handles.gscale_list,1)
            
            index=size(handles.listoffiles,2)+1;
            permID=permtable(j+(k-1)*size(handles.res_list,1));
            
            handles.listoffiles(index).source = strcat(handles.gammadir,handles.nonefacefoldername,'\',locallist(permID).name);
        
            handles.listoffiles(index).gscale = k;
            
            handles.listoffiles(index).res= j;
        
            handles.listoffiles(index).set = i;
            
            handles.listoffiles(index).name = locallist(permID).name;
            
            handles.listoffiles(index).category = 0;
        
        end
    end
    
lof = handles.listoffiles;


function [out] = disp_phosphene(in,out_size)

x_factor = round(out_size(2)/size(in,2));
y_factor = round(out_size(1)/size(in,1));

H= fspecial('gaussian',[y_factor x_factor],min(x_factor,y_factor)/6);

H=H/(max(max(H)));

for i = 1:size(in,1)
    
    for j = 1:size(in,2)
        
        scalar = double(in(i,j));
        
        out(((i-1)*y_factor+1):i*y_factor,((j-1)*x_factor+1):j*x_factor)= uint8(round(H*scalar));
        
        
    end
end

if (~isequal(size(out),out_size))

    if (size(out,2)<320)&&(size(out,1)<320)
       blank = zeros(320,320);
       blank(1:size(out,1),1:size(out,2))=out;
       out = blank;
    else
 
       x=size(out,2)-320;
       y=size(out,1)-320;
       blank = out( round(y/2)+1:size(out,1)-(y-round(y/2)), round(x/2)+1:size(out,2)-(x-round(y/2)));
       out = blank;       
    end
    
end




% --- Executes on button press in pb_exit.
function pb_exit_Callback(hObject, eventdata, handles)
% hObject    handle to pb_exit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close all;