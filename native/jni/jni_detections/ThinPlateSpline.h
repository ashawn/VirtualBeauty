
#ifndef THIN_PLATE_SPLINE_H_
#define THIN_PLATE_SPLINE_H_

#pragma once
#include <vector>
#include <Eigen/Core>
#include <Eigen/QR>

class ThinPlateSpline
{
public:
    ThinPlateSpline() {}
    ThinPlateSpline(std::vector<Eigen::Vector3d> &src, std::vector<Eigen::Vector3d> &dst);
    ~ThinPlateSpline() {}

    /* Solve */
    void solve();

    /* Interpolate */
    Eigen::Vector3d interpolate(const Eigen::Vector3d &p);

    /* Source Points */
    const std::vector<Eigen::Vector3d> &srcPoints() const { return mSrcPoints; }

    /* Set Source Points */
    void setSrcPoints(const std::vector<Eigen::Vector3d> &points) { mSrcPoints = points; }

    /* Destination Points */
    const std::vector<Eigen::Vector3d> &dstPoints() const { return mDstPoints; }

    /* Set Destination Points */
    void setDstPoints(const std::vector<Eigen::Vector3d> &points) { mDstPoints = points; }

protected:
    /* Radial Basis Function */
    static inline double radialBasis(double r) { return r == 0.0 ? r : r * r * log(r); }

    /* Data */
    std::vector<Eigen::Vector3d> mSrcPoints;
    std::vector<Eigen::Vector3d> mDstPoints;
    Eigen::MatrixXd mW;
    Eigen::MatrixXd mL;
};
#endif