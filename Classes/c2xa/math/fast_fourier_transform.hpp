
/**

    @file   c2xa/fast_fourier_transform.hpp
    @brief  c2xa/fast_fourier_transform

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/01/28

    fft4g.cのC++移植版(https://github.com/YSRKEN/Ooura-FFT-Library-by-Other-Language/)
    を更に修正したもの
    元ソース：
        京都大学助教授の大浦拓哉氏がフリーソフトとして提供する
        汎用 FFT (高速 フーリエ/コサイン/サイン 変換) パッケージ」
        (http://www.kurims.kyoto-u.ac.jp/~ooura/fft-j.html)
        のfft4g.c
    データ数N(2の冪乗)の1次元データに対して、離散フーリエ・コサイン・サイン変換を行う。
    内部でテーブルを利用するタイプで、インプレース型なので破壊的関数である。
    関数の使い方：
    void cdft(const int isgn, double *a);	//複素離散フーリエ変換
    void rdft(const int isgn, double *a);	//実数離散フーリエ変換
    void ddct(const int isgn, double *a);	//離散コサイン変換
    void ddst(const int isgn, double *a);	//離散サイン変換
    void dfct(double *a, double *t); //実対称フーリエ変換を用いたコサイン変換
    void dfst(double *a, double *t); //実非対称フーリエ変換を用いたサイン変換

*/
#pragma once
#ifndef C2XA_FAST_FOURIER_TRANSFORM_HPP
#define C2XA_FAST_FOURIER_TRANSFORM_HPP

namespace c2xa
{
    class fast_fourier_transform
    {
    private:
        int *ip;	//ビット反転に使用する作業領域
        double *w;	//cosとsinのテーブル(ip[0] == 0だと初期化される)
        int n;		//配列の要素数(2N)

    public:
        fast_fourier_transform( const int );
        ~fast_fourier_transform();

    private:
        void makewt( int nw, int *ip, double *w );
        void makect( int nc, int *ip, double *c );
        void bitrv2( int n, int *ip, double *a );
        void bitrv2conj( int n, int *ip, double *a );
        void cftfsub( int n, double *a, double *w );
        void cftbsub( int n, double *a, double *w );
        void cft1st( int n, double *a, double *w );
        void cftmdl( int n, int l, double *a, double *w );
        void rftfsub( int n, double *a, int nc, double *c );
        void rftbsub( int n, double *a, int nc, double *c );
        void dctsub( int n, double *a, int nc, double *c );
        void dstsub( int n, double *a, int nc, double *c );

    public:
        void cdft( int isgn, double *a );	//複素離散フーリエ変換
        void rdft( int isgn, double *a );	//実数離散フーリエ変換
        void ddct( int isgn, double *a );	//離散コサイン変換
        void ddst( int isgn, double *a );	//離散サイン変換
        void dfct( double *a, double *t );	//実対称フーリエ変換を用いたコサイン変換
        void dfst( double *a, double *t );	//実非対称フーリエ変換を用いたサイン変換
    };
    using fft = fast_fourier_transform;
    template< size_t size_ >
    static fft* get_fft()
    {
        static fft fft_{ size_ };
        return &fft_;
    }
}

#endif//C2XA_FAST_FOURIER_TRANSFORM_HPP