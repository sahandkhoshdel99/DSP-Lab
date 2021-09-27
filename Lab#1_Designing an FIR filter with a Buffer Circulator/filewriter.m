function filewriter(in, filename)
fileID = fopen(filename, 'w');
fprintf(fileID, '%.4f ', in);
fclose(fileID);
end

