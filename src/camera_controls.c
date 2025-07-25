/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:29:14 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 15:29:19 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i d);
static inline void	pan(t_context *ctx, t_cam *cam, t_vec2i d, t_vec2i pos);
static inline void	zoom(t_context *ctx, t_vec2i pos);

/**
 * Processes all camera-related input and updates the camera state.
 *
 * - Translation and rotation	(WASD / ARROW keys).
 *
 * - Orbiting around a target	(Left ALT + Left Mouse Button).
 *
 * - Panning the camera			(Left ALT + Middle Mouse Button).
 *
 * - Zooming in/out				(Left ALT + Right Mouse Button).
 *
 * With active camera motion, `update_camera()` is called to recalculate
 * the camera's `eye` position from updated pitch, yaw, and distance.
 *
 * Uses a logical mouse position that wraps around the screen for
 * zooming and orbiting.
 *
 * @param param Render context containing the camera.
 */
void	control_camera(void *param)
{
	static t_vec2i	lgc_m;
	t_context		*ctx;
	t_vec2i			pos;

	ctx = param;
	if (ctx->cam.projection != ISOMETRIC)
	{
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (ctx->cam.orbiting || ctx->cam.zooming)
			lgc_m = vec2i(wrap_m_x(ctx, &pos), wrap_m_y(ctx, &pos));
		if (ctx->spin_mode == OFF)
			translate_rotate(ctx);
		if (!ctx->cam.zooming && !ctx->cam.panning)
			orbit(ctx, vec2i_add(pos, lgc_m), vec2i(0, 0));
		if (!ctx->cam.orbiting && !ctx->cam.panning)
			zoom(ctx, vec2i_add(pos, lgc_m));
		if (!ctx->cam.orbiting && !ctx->cam.zooming)
			pan(ctx, &ctx->cam, vec2i(0, 0), pos);
		if (!(ctx->cam.orbiting || ctx->cam.zooming || ctx->cam.panning))
			return (mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL));
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		update_camera(&ctx->cam);
	}
}

/**
 * Orbits the camera around the target `Left ALT + Left Mouse Button` is held.
 *
 * Calculates mouse deltas and adjusts the camera's `yaw` and `pitch` angles.
 *
 * Frame-rate independent.
 *
 * @param ctx Rendering context containing camera and mlx context.
 * @param pos Logical mouse position.
 * @param d Auxialiry delta vec2i.
 */
static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i d)
{
	static t_vec2i	prev;
	static t_vec2i	orig;
	float			speed;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		if (!ctx->cam.orbiting)
			mlx_get_mouse_pos(ctx->mlx, &orig.x, &orig.y);
		else
		{
			d = vec2i_sub(pos, prev);
			speed = ctx->mlx->delta_time * 60.0f * ORBIT_SENS;
			ctx->cam.yaw -= d.x * speed;
			ctx->cam.pitch += d.y * speed;
		}
		ctx->cam.orbiting = true;
		prev = pos;
	}
	else
	{
		if (ctx->cam.orbiting)
			mlx_set_mouse_pos(ctx->mlx, (uint32_t)orig.x, (uint32_t)orig.y);
		ctx->cam.orbiting = false;
	}
}

/**
 * Pans the camera target when `Left ALT + Middle Mouse Button` is held.
 *
 * Calculates the x and y movement of the mouse and moves the camera's `target`
 * accordingly. Panning speed is proportional to `cam->distance`.
 *
 * Frame-rate independent.
 *
 * @param ctx Rendering context containing mlx context.
 * @param cam Camera.
 * @param d Mouse movement delta.
 * @param pos Current mouse position.
 */
static inline void	pan(t_context *ctx, t_cam *cam, t_vec2i d, t_vec2i pos)
{
	static t_vec2i	prev;
	float			speed;
	t_vec3			forward;
	t_vec3			right;
	t_vec3			up;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
	{
		if (cam->panning)
			d = vec2i_sub(pos, prev);
		cam->panning = true;
		prev = pos;
		speed = fmaxf(cam->distance, 1.0f);
		speed *= ctx->mlx->delta_time * 60.0f * PAN_SENS;
		forward = vec3_normalize(vec3_sub(cam->target, cam->eye));
		right = vec3_normalize(vec3_cross(forward, cam->up));
		up = vec3_normalize(vec3_cross(right, forward));
		cam->target = vec3_add(cam->target, vec3_scale(right, -d.x * speed));
		cam->target = vec3_add(cam->target, vec3_scale(up, d.y * speed));
	}
	else
		cam->panning = false;
}

/**
 * Zooms the camera in/out when `Left ALT + Right Mouse Button` is held.
 *
 * Measures vertical mouse movement and adjusts `cam.distance`,
 * clamped to a minimum of `0.1f`. Updates `ortho_size` proportionally.
 *
 * Frame-rate independent.
 *
 * @param ctx Rendering context containing camera and mlx context.
 * @param pos Logical mouse position.
 */
static inline void	zoom(t_context *ctx, t_vec2i pos)
{
	static t_vec2i	prev;
	static t_vec2i	orig;
	float			speed;
	float			delta;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		if (!ctx->cam.zooming)
			mlx_get_mouse_pos(ctx->mlx, &orig.x, &orig.y);
		speed = ctx->cam.distance * ctx->mlx->delta_time * 60.0f * ZOOM_SENS;
		delta = pos.y - prev.y;
		ctx->cam.distance = fmaxf(0.1f, ctx->cam.distance - delta * speed);
		ctx->cam.ortho_size = ctx->cam.distance * 0.5f;
		prev.y = pos.y;
		ctx->cam.zooming = true;
	}
	else
	{
		if (ctx->cam.zooming)
			mlx_set_mouse_pos(ctx->mlx, (uint32_t)orig.x, (uint32_t)orig.y);
		prev.y = pos.y;
		ctx->cam.zooming = false;
	}
}
