/*
    csdebug.c:

    Copyright (C) 2013 Andres Cabrera

    This file is part of Csound.

    The Csound Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    Csound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/

#include <assert.h>

#include "csdebug.h"

#include "csoundCore.h"

PUBLIC void csoundDebuggerInit(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *)malloc(sizeof(csdebug_data_t));
    data->bkpt_anchor = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    data->bkpt_anchor->line = -1;
    data->bkpt_anchor->next = NULL;
    data->debug_instr_ptr = NULL;
    data->status = CSDEBUG_STATUS_RUNNING;
    data->bkpt_buffer = csoundCreateCircularBuffer(csound, 64, sizeof(bkpt_node_t **));
    data->cmd_buffer = csoundCreateCircularBuffer(csound, 64, sizeof(debug_command_t));
    csound->csdebug_data = data;
    csound->kperf = csound->kperf_debug;
}

PUBLIC void csoundDebuggerClean(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    bkpt_node_t *node = data->bkpt_anchor;
    csoundDestroyCircularBuffer(csound, data->bkpt_buffer);
    csoundDestroyCircularBuffer(csound, data->cmd_buffer);
    while (node) {
        bkpt_node_t *oldnode = node;
        node = node->next;
        free(oldnode);
    }
    free(data);
    csound->csdebug_data = NULL;
    csound->kperf = csound->kperf_nodebug;
}

PUBLIC void csoundDebugStart(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    data->status = CSDEBUG_STATUS_RUNNING;
}

PUBLIC void csoundSetBreakpoint(CSOUND *csound, int line, int skip)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    if (line < 0) {
        csound->Warning(csound, Str("Negative line for breakpoint invalid."));
    }
    bkpt_node_t *newpoint = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    newpoint->line = line;
    newpoint->instr = 0;
    newpoint->skip = skip;
    newpoint->count = skip;
    newpoint->mode = CSDEBUG_BKPT_LINE;
    csoundWriteCircularBuffer(csound, data->bkpt_buffer, &newpoint, 1);
}

PUBLIC void csoundRemoveBreakpoint(CSOUND *csound, int line)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    if (line < 0) {
        csound->Warning(csound, Str ("Negative line for breakpoint invalid."));
    }
    bkpt_node_t *newpoint = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    newpoint->line = line;
    newpoint->instr = 0;
    newpoint->mode = CSDEBUG_BKPT_DELETE;
    csoundWriteCircularBuffer(csound, data->bkpt_buffer, &newpoint, 1);
}

PUBLIC void csoundSetInstrumentBreakpoint(CSOUND *csound, MYFLT instr, int skip)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    bkpt_node_t *newpoint = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    newpoint->line = -1;
    newpoint->instr = instr;
    newpoint->skip = skip;
    newpoint->count = skip;
    newpoint->mode = CSDEBUG_BKPT_INSTR;
    csoundWriteCircularBuffer(csound, data->bkpt_buffer, &newpoint, 1);
}

PUBLIC void csoundRemoveInstrumentBreakpoint(CSOUND *csound, MYFLT instr)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    bkpt_node_t *newpoint = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    newpoint->line = -1;
    newpoint->instr = instr;
    newpoint->mode = CSDEBUG_BKPT_DELETE;
    csoundWriteCircularBuffer(csound, data->bkpt_buffer, &newpoint, 1);
}

PUBLIC void csoundClearBreakpoints(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    bkpt_node_t *newpoint = (bkpt_node_t *) malloc(sizeof(bkpt_node_t));
    newpoint->line = -1;
    newpoint->instr = -1;
    newpoint->mode = CSDEBUG_BKPT_CLEAR_ALL;
    csoundWriteCircularBuffer(csound, data->bkpt_buffer, &newpoint, 1);
}

PUBLIC void csoundSetBreakpointCallback(CSOUND *csound, breakpoint_cb_t bkpt_cb, void *userdata)
{

    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    data->bkpt_cb = bkpt_cb;
    data->cb_data = userdata;
}

PUBLIC void csoundDebugStepOver(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    debug_command_t command = CSDEBUG_CMD_STEPOVER;
    csoundWriteCircularBuffer(csound, data->cmd_buffer, &command, 1);
}

PUBLIC void csoundDebugStepInto(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    debug_command_t command = CSDEBUG_CMD_STEPINTO;
    csoundWriteCircularBuffer(csound, data->cmd_buffer, &command, 1);
}

PUBLIC void csoundDebugNext(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    debug_command_t command = CSDEBUG_CMD_NEXT;
    csoundWriteCircularBuffer(csound, data->cmd_buffer, &command, 1);
}

PUBLIC void csoundDebugContinue(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    debug_command_t command = CSDEBUG_CMD_CONTINUE;
    csoundWriteCircularBuffer(csound, data->cmd_buffer, &command, 1);
}

PUBLIC void csoundDebugStop(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    debug_command_t command = CSDEBUG_CMD_STOP;
    csoundWriteCircularBuffer(csound, data->cmd_buffer, &command, 1);
}

PUBLIC INSDS *csoundDebugGetInstrument(CSOUND *csound)
{
    csdebug_data_t *data = (csdebug_data_t *) csound->csdebug_data;
    assert(data);
    return data->debug_instr_ptr;
}
