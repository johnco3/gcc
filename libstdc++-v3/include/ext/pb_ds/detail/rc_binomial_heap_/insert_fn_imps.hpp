// -*- C++ -*-

// Copyright (C) 2005-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/**
 * @file rc_binomial_heap_/insert_fn_imps.hpp
 * Contains an implementation for rc_binomial_heap_.
 */

#ifdef PB_DS_CLASS_C_DEC

PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::point_iterator
PB_DS_CLASS_C_DEC::
push(const_reference r_val)
{
  PB_DS_ASSERT_VALID((*this))

  make_0_exposed();

  PB_DS_ASSERT_VALID((*this))

  node_pointer p_nd = base_type::get_new_node_for_insert(r_val);

  p_nd->m_p_l_child = p_nd->m_p_prev_or_parent = 0;
  p_nd->m_metadata = 0;

  if (base_type::m_p_max == 0 || Cmp_Fn::operator()(base_type::m_p_max->m_value, r_val))
    base_type::m_p_max = p_nd;

  p_nd->m_p_next_sibling = base_type::m_p_root;

  if (base_type::m_p_root != 0)
    base_type::m_p_root->m_p_prev_or_parent = p_nd;

  base_type::m_p_root = p_nd;

  if (p_nd->m_p_next_sibling != 0&&  p_nd->m_p_next_sibling->m_metadata == 0)
    m_rc.push(p_nd);

  PB_DS_ASSERT_VALID((*this))

  return point_iterator(p_nd);
}

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
modify(point_iterator it, const_reference r_new_val)
{
  PB_DS_ASSERT_VALID((*this))

  make_binomial_heap();

  base_type::modify(it, r_new_val);

  base_type::find_max();

  PB_DS_ASSERT_VALID((*this))
}

PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::node_pointer
PB_DS_CLASS_C_DEC::
link_with_next_sibling(node_pointer p_nd)
{
  node_pointer p_next = p_nd->m_p_next_sibling;

  _GLIBCXX_DEBUG_ASSERT(p_next != 0);
  _GLIBCXX_DEBUG_ASSERT(p_next->m_p_prev_or_parent == p_nd);

  if (Cmp_Fn::operator()(p_nd->m_value, p_next->m_value))
    {
      p_next->m_p_prev_or_parent = p_nd->m_p_prev_or_parent;

      if (p_next->m_p_prev_or_parent == 0)
	base_type::m_p_root = p_next;
      else
	p_next->m_p_prev_or_parent->m_p_next_sibling = p_next;

      if (base_type::m_p_max == p_nd)
	base_type::m_p_max = p_next;

      base_type::make_child_of(p_nd, p_next);

      ++p_next->m_metadata;

      return p_next;
    }

  p_nd->m_p_next_sibling = p_next->m_p_next_sibling;

  if (p_nd->m_p_next_sibling != 0)
    p_nd->m_p_next_sibling->m_p_prev_or_parent = p_nd;

  if (base_type::m_p_max == p_next)
    base_type::m_p_max = p_nd;

  base_type::make_child_of(p_next, p_nd);

  ++p_nd->m_metadata;

  return p_nd;
}

PB_DS_CLASS_T_DEC
void
PB_DS_CLASS_C_DEC::
make_0_exposed()
{
  if (m_rc.empty())
    return;

  node_pointer p_nd = m_rc.top();

  m_rc.pop();

  _GLIBCXX_DEBUG_ASSERT(p_nd->m_p_next_sibling != 0);
  _GLIBCXX_DEBUG_ASSERT(p_nd->m_metadata == p_nd->m_p_next_sibling->m_metadata);

  node_pointer p_res = link_with_next_sibling(p_nd);

  if (p_res->m_p_next_sibling != 0&&  p_res->m_metadata == p_res->m_p_next_sibling->m_metadata)
    m_rc.push(p_res);
}
#endif
