Dir.glob('*_o.txt') do |f|
  s = File.read(f)
  s = s.chomp()
  File.open(f, mode = "w", perm=0644) do |fl|
    fl.write(s)
  end
end
