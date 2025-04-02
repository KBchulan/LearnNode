#include <cmath>
#include <fstream>

int main(int argc, char *argv[])
{
  std::puts(std::to_string(argc).c_str());
  
  std::ofstream fout(argv[1]);

  fout << "#ifndef TABLE_H\n"
       << "#define TABLE_H\n"
       << "static const double sqrtTable[] = {\n";

  for (int i = 0; i <= 100; ++i)
  {
    fout << "  " << std::sqrt(static_cast<double>(i));
    if (i < 100)
    {
      fout << ",";
    }
    fout << "  // sqrt(" << i << ")\n";
  }

  fout << "};\n\n#endif\n";

  return 0;
}