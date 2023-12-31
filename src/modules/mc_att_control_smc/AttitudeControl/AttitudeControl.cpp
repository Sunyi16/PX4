/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file AttitudeControl.cpp
 */

#include <AttitudeControl.hpp>

#include <mathlib/math/Functions.hpp>

#include<lib/adrc/adrc.h>

using namespace matrix;

matrix::Vector3f AttitudeControl::update(matrix::Quatf q, matrix::Quatf qd, Vector3f rate,float dt)
{
	// ensure input quaternions are exactly normalized because acosf(1.00001) == NaN
	q.normalize();
	qd.normalize();

		// calculate reduced desired attitude neglecting vehicle's yaw to prioritize roll and pitch
/* 	const Vector3f e_z = q.dcm_z();//计算z轴单位向量
	const Vector3f e_z_d = qd.dcm_z();
	Quatf qd_red(e_z, e_z_d);//计算去除旋转误差后仅代表倾斜误差的四元数

	if (fabsf(qd_red(1)) > (1.f - 1e-5f) || fabsf(qd_red(2)) > (1.f - 1e-5f)) {
		// In the infinitesimal corner case where the vehicle and thrust have the completely opposite direction,
		// full attitude control anyways generates no yaw input and directly takes the combination of
		// roll and pitch leading to the correct desired yaw. Ignoring this case would still be totally safe and stable.
		qd_red = qd;

	} else {
		// transform rotation from current to desired thrust vector into a world frame reduced desired attitude
		qd_red *= q;
	}

	// mix full and reduced desired attitude
	Quatf q_mix = qd_red.inversed() * qd;
	q_mix.canonicalize();
	// catch numerical problems with the domain of acosf and asinf
	q_mix(0) = math::constrain(q_mix(0), -1.f, 1.f);
	q_mix(3) = math::constrain(q_mix(3), -1.f, 1.f);
	qd = qd_red * Quatf(cosf(_yaw_w * acosf(q_mix(0))), 0, 0, sinf(_yaw_w * asinf(q_mix(3)))); */

	Vector3f att_sp;
	att_sp(0)=Eulerf(Quatf(qd)).phi();
	att_sp(1)=Eulerf(Quatf(qd)).theta();
	att_sp(2)=Eulerf(Quatf(qd)).psi();
	Vector3f att;
	att(0)=Eulerf(Quatf(q)).phi();
	att(1)=Eulerf(Quatf(q)).theta();
	att(2)=Eulerf(Quatf(q)).psi();
	Vector3f torque;
	static Vector3f last_torque;

	Fhan_Data *esc_smc;
	//float dt=0.00001;
	//PX4_INFO("att_sp: %f  att: %f",(double)att_sp(2),(double)att(2));
	static class smc Smc;
	torque=Smc.SMC_Control(att_sp,att,rate,dt);
	last_torque=torque;

	return torque;
}
