Data = importdata('./output.dat');
S = size(Data);
ROW = S(1);
COL = S(2);

% for test
TimeStep = 1;
TimeGap = 10;

Range = 5e08;

N = sqrt(COL)
NX = N
NY = N

[X,Y] = meshgrid(1:NX,1:NY);

SurfStep = ones(NY,NX);

for a = 1:NY
    for b = 1:NX
        SurfStep(a,b)=Data(TimeStep,a*NX+b-NX);
    end
end


% make .gif 

figure
%plot UX
subplot(1,1,1);
%surf(X,Y,SurfStep)
mesh(X,Y,SurfStep)
colorbar
caxis([-Range Range])
axis([1 NX 1 NY -Range Range])

ax = gca;
ax.NextPlot = 'replaceChildren';
f=getframe(gcf);  
imind=frame2im(f);
[imind,cm] = rgb2ind(imind,256);
imwrite(imind,cm,'FigureUY.gif','gif', 'Loopcount',inf,'DelayTime',0.2);

% loop for gif

loops = ROW;
F(loops) = struct('cdata',[],'colormap',[]);

for j = TimeStep+TimeGap:TimeGap:loops
    TimeStep = j
        for a = 1:NY
            for b = 1:NX
                SurfStep(a,b)=Data(TimeStep,a*NX+b-NX);
            end
        end
        
%plot UX
subplot(1,1,1);
%surf(X,Y,SurfStep)
mesh(X,Y,SurfStep)
colorbar
caxis([-Range Range])
axis([1 NX 1 NY -Range Range])



    drawnow
    F(j) = getframe;
    f=getframe(gcf);  
    imind=frame2im(f);
    [imind,cm] = rgb2ind(imind,256);
    imwrite(imind,cm,'FigureUY.gif','gif','WriteMode','append','DelayTime',0.2);
end
