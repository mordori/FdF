/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controller.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:29:14 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/24 01:06:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i d);
static inline void	pan(t_context *ctx, t_cam *cam, t_vec2i d, t_vec2i pos);
static inline void	zoom(t_context *ctx, t_vec2i pos);
static inline void	translate_rotate(t_context *ctx);

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
		if (ctx->spin == OFF)
			translate_rotate(ctx);
		if (!ctx->cam.zooming && !ctx->cam.panning)
			orbit(ctx, vec2i_add(pos, lgc_m), vec2i(0, 0));
		if (!ctx->cam.orbiting && !ctx->cam.panning)
			zoom(ctx, vec2i_add(pos, lgc_m));
		if (!ctx->cam.orbiting && !ctx->cam.zooming)
			pan(ctx, &ctx->cam, vec2i(0, 0), pos);
		if (!ctx->cam.orbiting && !ctx->cam.zooming && !ctx->cam.panning)
			return (mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL));
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		update_camera(&ctx->cam);
	}
}

/**
 * Translates and rotates the model based on keyboard input.
 *
 * Movement and rotation speeds are frame-rate independent.
 *
 * @param ctx Rendering context containing model transform and mlx context.
 */
static inline void	translate_rotate(t_context *ctx)
{
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_W))
		ctx->transform.pos.z -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_S))
		ctx->transform.pos.z += ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_A))
		ctx->transform.pos.x -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_D))
		ctx->transform.pos.x += ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_UP))
		ctx->transform.rot.x -= ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN))
		ctx->transform.rot.x += ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
		ctx->transform.rot.z += ctx->mlx->delta_time;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT))
		ctx->transform.rot.z -= ctx->mlx->delta_time;
}

/**
 * Orbits the camera around the target `Left ALT + Left Mouse Button` is held.
 *
 * Calculates mouse deltas and adjusts the camera's `yaw` and `pitch` angles.
 *
 * Frame-rate independent.
 *
 * @param ctx Rendering context containing camera and mlx context.
 */
static inline void	orbit(t_context *ctx, t_vec2i pos, t_vec2i d)
{
	static t_vec2i	prev;
	static t_vec2i	orig;
	float			speed;

	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		if (ctx->cam.orbiting)
		{
			d = vec2i_sub(pos, prev);
			speed = ctx->mlx->delta_time * 60.0f * ORBIT_SENS;
			ctx->cam.yaw -= d.x * speed;
			ctx->cam.pitch += d.y * speed;
		}
		else
			mlx_get_mouse_pos(ctx->mlx, &orig.x, &orig.y);
		prev = pos;
		ctx->cam.orbiting = true;
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
		if (prev.x >= 0 && prev.y >= 0)
			d = vec2i_sub(pos, prev);
		speed = cam->distance * ctx->mlx->delta_time * 60.0f * PAN_SENS;
		forward = vec3_normalize(vec3_sub(cam->target, cam->eye));
		right = vec3_normalize(vec3_cross(forward, cam->up));
		up = vec3_normalize(vec3_cross(right, forward));
		cam->target = vec3_add(cam->target, vec3_scale(right, -d.x * speed));
		cam->target = vec3_add(cam->target, vec3_scale(up, d.y * speed));
		prev = pos;
		cam->panning = true;
	}
	else
	{
		cam->panning = false;
		prev = vec2i_n(-1);
	}
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
 * @param pos
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
		{
			mlx_get_mouse_pos(ctx->mlx, &orig.x, &orig.y);
			prev.y = pos.y;
		}
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
		ctx->cam.zooming = false;
	}
}
