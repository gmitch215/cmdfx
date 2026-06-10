class Cmdfx < Formula
  desc "Lightweight cross-platform 2D terminal game engine"
  homepage "https://github.com/gmitch215/cmdfx"
  url "https://github.com/gmitch215/cmdfx/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "0000000000000000000000000000000000000000000000000000000000000000"
  license "MIT"
  head "https://github.com/gmitch215/cmdfx.git", branch: "master"

  depends_on "cmake" => :build
  depends_on "ncurses"

  on_linux do
    depends_on "alsa-lib"
  end

  def install
    system "cmake", "-S", ".", "-B", "build",
           "-DKN_CMDFX=OFF",
           "-DTEST_CMDFX=OFF",
           "-DDOCS_CMDFX=OFF",
           "-DPACKAGE_CMDFX=OFF",
           *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    (testpath/"CMakeLists.txt").write <<~CMAKE
      cmake_minimum_required(VERSION 3.30)
      project(cmdfx_test C)
      find_package(cmdfx REQUIRED)
      add_executable(cmdfx_test test.c)
      target_link_libraries(cmdfx_test PRIVATE cmdfx::cmdfx)
    CMAKE
    (testpath/"test.c").write <<~C
      #include <cmdfx.h>
      int main(void) { return Canvas_getWidth() >= 0 ? 0 : 1; }
    C
    system "cmake", "-S", testpath, "-B", testpath/"build",
           "-DCMAKE_PREFIX_PATH=#{prefix}"
    system "cmake", "--build", testpath/"build"
    system testpath/"build"/"cmdfx_test"
  end
end
