/*
 * SpectRE - A Spectral Code for Reheating
 * Copyright (C) 2009-2010 Hal Finkel, Nathaniel Roth and Richard Easther
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Second-order Verlet integrator.
 */

#ifndef VERLET_HPP
#define VERLET_HPP

#include "model_params.hpp"
#include "time_state.hpp"
#include "field.hpp"
#include "integrator.hpp"
#include "nonlinear_transformer.hpp"
#include "v_integrator.hpp"
#include "fft.hpp"

template <typename R>
class verlet : public integrator<R>
{
public:
	verlet(model_params &mp_, time_state<R> &ts_,
	       field<R> &phi_, field<R> &phidot_,
	       IF_CHI_ARG(field<R> &chi_,)
	       IF_CHI_ARG(field<R> &chidot_,)
	       fft_worker<R> &fft_plans)
		: mp(mp_), ts(ts_), phi(phi_), phidot(phidot_), IF_CHI_ARG(chi(chi_),)
		  IF_CHI_ARG(chidot(chidot_),)
		phiddot("phiddot"), phidot_staggered("phidot_staggered"),
		IF_CHI_ARG(chiddot("chiddot"),)
		IF_CHI_ARG(chidot_staggered("chidot_staggered"),)
		nlt(ts_, fft_plans), vi(),
		addot(0), adot_staggered(0), dptdt(1./RESCALE_B), ddptdt(0), dptdt_staggered(0)
		{
			phiddot.construct(fft_plans);
			phidot_staggered.construct(fft_plans);

			IF_CHI(chiddot.construct(fft_plans);
			       chidot_staggered.construct(fft_plans));
		}

public:	
	virtual void step();
	virtual void initialize();

protected:
	model_params &mp;
	time_state<R> &ts;

	field<R> &phi, &phidot, phiddot, phidot_staggered;
	IF_CHI(field<R> &chi;
	       field<R> &chidot;
	       field<R> chiddot;
	       field<R> chidot_staggered;)
	nonlinear_transformer<R> nlt;
	v_integrator<R> vi;
	R addot, adot_staggered;
	R dptdt, ddptdt, dptdt_staggered;	
};

#endif // VERLET_HPP
