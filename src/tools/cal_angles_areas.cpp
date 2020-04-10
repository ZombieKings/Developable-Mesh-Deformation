﻿#include "cal_angles_areas.h"

void Zombie::cal_angles(const Eigen::Matrix3Xd& V,
	const Eigen::MatrixXi& F,
	Eigen::VectorXd& vecAngles)
{
	const int DIM = F.rows();
	vecAngles.resize(V.cols());
	vecAngles.setZero();
	for (int f = 0; f < F.cols(); ++f)
	{
		const Eigen::VectorXi& fv = F.col(f);
		for (int vi = 0; vi < DIM; ++vi)
		{
			const Eigen::VectorXd& p0 = V.col(fv[vi]);
			const Eigen::VectorXd& p1 = V.col(fv[(vi + 1) % DIM]);
			const Eigen::VectorXd& p2 = V.col(fv[(vi + 2) % DIM]);
			const double angle = std::acos(std::max(-1.0, std::min(1.0, (p1 - p0).normalized().dot((p2 - p0).normalized()))));
			vecAngles(F(vi, f)) += angle;
		}
	}
}

void Zombie::cal_angles(const Eigen::Matrix3Xd& V,
	const Eigen::MatrixXi& F,
	const Eigen::VectorXi& interVidx,
	Eigen::VectorXd& vecAngles,
	Eigen::Matrix3Xd& matAngles)
{
	const int DIM = F.rows();
	matAngles.setConstant(DIM, F.cols(), 0);
	vecAngles.setConstant(V.cols(), 0);
	for (int f = 0; f < F.cols(); ++f)
	{
		const Eigen::VectorXi& fv = F.col(f);
		for (int vi = 0; vi < DIM; ++vi)
		{
			const Eigen::VectorXd& p0 = V.col(fv[vi]);
			const Eigen::VectorXd& p1 = V.col(fv[(vi + 1) % DIM]);
			const Eigen::VectorXd& p2 = V.col(fv[(vi + 2) % DIM]);
			const double angle = std::acos(std::max(-1.0, std::min(1.0, (p1 - p0).normalized().dot((p2 - p0).normalized()))));
			matAngles(vi, f) = angle;
			if (interVidx(fv(vi)) != -1)
				vecAngles(fv(vi)) += angle;
		}
	}
}

void Zombie::cal_angles_and_areas(const Eigen::Matrix3Xd& V,
	const Eigen::MatrixXi& F,
	const Eigen::VectorXi& interVidx,
	Eigen::VectorXd& vecAngles,
	Eigen::VectorXd& areas,
	Eigen::Matrix3Xd& matAngles)
{
	const int DIM = F.rows();
	matAngles.setConstant(DIM, F.cols(), 0);
	vecAngles.setConstant(V.cols(), 0);
	areas.setConstant(V.cols(), 0);
	for (int f = 0; f < F.cols(); ++f)
	{
		const Eigen::VectorXi& fv = F.col(f);
		//Mix area
		const double area = (V.col(fv[1]) - V.col(fv[0])).cross(V.col(fv[2]) - V.col(fv[0])).norm() / 6.0;

		for (int vi = 0; vi < DIM; ++vi)
		{
			const Eigen::VectorXd& p0 = V.col(fv[vi]);
			const Eigen::VectorXd& p1 = V.col(fv[(vi + 1) % DIM]);
			const Eigen::VectorXd& p2 = V.col(fv[(vi + 2) % DIM]);
			const double angle = std::acos(std::max(-1.0, std::min(1.0, (p1 - p0).normalized().dot((p2 - p0).normalized()))));
			matAngles(vi, f) = angle;
			if (interVidx(fv(vi)) != -1)
			{
				areas(fv[vi]) += area;
				vecAngles(fv(vi)) += angle;
			}
		}
	}
}