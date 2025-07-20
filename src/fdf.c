/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 06:50:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	loop(void *param);
static inline void	input(mlx_key_data_t keydata, void *param);
static inline void	controls(void *param);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_context	*ctx;

	if (argc != 2)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_FULLSCREEN, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_mlx_error(NULL);
	img  = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_mlx_error(mlx);
	if (initialize(argv[1], &ctx, mlx, img) == ERROR)
		ft_mlx_error(mlx);
	mlx_key_hook(mlx, input, ctx);
	mlx_loop_hook(mlx, loop, ctx);
	mlx_close_hook(mlx, on_close, ctx);
	mlx_resize_hook(mlx, on_resize, ctx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	fdf_free(ctx->verts, ctx->tris, ctx);
	return (EXIT_SUCCESS);
}

static inline void	loop(void *param)
{
	static double	time_color;
	t_context		*ctx;
	double			t;

	ctx = param;
	controls(ctx);
	t = ctx->time_rot;
	if (ctx->colors == AMAZING)
	{
		ctx->color1 = rainbow_rgb(wrapf(time_color * 2.0f));
		ctx->color2 = rainbow_rgb(wrapf(time_color * 2.0f + 1.0f));
	}
	if (ctx->spin == ON)
	{
		ctx->transform.rot = vec3((t * 0.5f), sinf(t * 0.1f), cosf(t*0.2f) - 1);
		ctx->time_rot += ctx->mlx->delta_time;
	}
	time_color += ctx->mlx->delta_time;
	render(ctx);
}

static inline void	input(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (!ctx->mlx)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_RELEASE)
	{
		if (ctx->cam.projection == ISOMETRIC)
			ctx->cam.projection = ORTHOGRAPHIC;
		else if (ctx->cam.projection == ORTHOGRAPHIC)
			ctx->cam.projection = PERSPECTIVE;
		else if (ctx->cam.projection == PERSPECTIVE)
		{
			ctx->spin = OFF;
			ctx->cam.projection = ISOMETRIC;
			reset_transforms(ctx);
		}
	}
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
		frame(ctx);
	if (keydata.key == MLX_KEY_C && keydata.action == MLX_RELEASE)
		ctx->colors = !ctx->colors;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE && \
		ctx->cam.projection != ISOMETRIC)
		ctx->spin = !ctx->spin;
	if (keydata.key == MLX_KEY_R && keydata.action == MLX_RELEASE)
		reset_transforms(ctx);
}

static inline void pan(t_cam *cam, float dx, float dy)
{
	float	speed;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	speed = cam->distance * 0.0016f;
	forward = vec3_normalize(vec3_sub(cam->target, cam->eye));
	right = vec3_normalize(vec3_cross(forward, cam->up));
	up = vec3_normalize(vec3_cross(right, forward));
	cam->target = vec3_add(cam->target, vec3_scale(right, -dx * speed));
	cam->target = vec3_add(cam->target, vec3_scale(up, dy * speed));
	cam->eye = vec3_add(cam->eye, vec3_scale(right, -dx * speed));
	cam->eye = vec3_add(cam->eye, vec3_scale(up, dy * speed));
}

static inline void zoom(t_cam *cam, float delta)
{
	float	speed;

	speed = cam->distance * 0.1f;
	cam->distance = fmaxf(0.1f, cam->distance - delta * speed);
	cam->ortho_size = cam->distance * 0.5f;
}

static inline void	controls(void *param)
{
	t_context	*ctx;
	t_vec2i		pos;
	t_vec2i		d;
	float		sensitivity = 0.005f;
	static double prev_x = -1;
	static double prev_y = -1;
	static bool zooming = false;

	d = vec2i(0, 0);
	ctx = param;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT) && \
		ctx->cam.projection != ISOMETRIC)
	{
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (prev_x >= 0 && prev_y >= 0)
		{
			d.x = pos.x - prev_x;
			d.y = pos.y - prev_y;
			ctx->cam.yaw -= d.x * sensitivity;
			ctx->cam.pitch += d.y * sensitivity;
		}
		prev_x = pos.x;
		prev_y = pos.y;
		//mlx_set_mouse_pos(ctx->mlx, ctx->mlx->width / 2, ctx->mlx->height / 2);
		update_camera(&ctx->cam);
	}
	else if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (!zooming) { prev_y = pos.y; zooming = true; }
		float delta = (pos.y - prev_y) * 0.05f;
		zoom(&ctx->cam, delta);
		prev_y = pos.y;
		update_camera(&ctx->cam);
	}
	else if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
	{
		mlx_get_mouse_pos(ctx->mlx, &pos.x, &pos.y);
		if (prev_x >= 0 && prev_y >= 0)
		{
			d.x = pos.x - prev_x;
			d.y = pos.y - prev_y;
		}
		pan(&ctx->cam, d.x, d.y);
		prev_x = pos.x;
		prev_y = pos.y;
		update_camera(&ctx->cam);
	}
	else
	{
		prev_x = -1;
		prev_y = -1;
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
		zooming = false;
	}
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_W))
		ctx->transform.pos.z -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_S))
		ctx->transform.pos.z += ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_A))
		ctx->transform.pos.x -= ctx->mlx->delta_time * 4.0f;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_D))
		ctx->transform.pos.x += ctx->mlx->delta_time * 4.0f;

}
