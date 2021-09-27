function output = File_reader(fs, filename)
fileID = fopen(filename, 'r');
output = fscanf(fileID, '%f ',[fs, 1]);
fclose(fileID);
end

